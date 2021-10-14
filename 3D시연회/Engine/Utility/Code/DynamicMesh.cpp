#include "DynamicMesh.h"

USING(Engine)

Engine::CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pAniCtrl(nullptr)
{

}

Engine::CDynamicMesh::CDynamicMesh(const CDynamicMesh& rhs)
	: CComponent(rhs)
	, m_pRootFrame(rhs.m_pRootFrame)
	, m_pLoader(rhs.m_pLoader)
	, m_MeshcontainerList(rhs.m_MeshcontainerList)
{
	m_pAniCtrl = CAniCtrl::Create(*rhs.m_pAniCtrl);
}

Engine::CDynamicMesh::~CDynamicMesh(void)
{

}

void CDynamicMesh::Set_AnimationIndex(const _uint & iIndex)
{
	m_pAniCtrl->Set_AnimationIndex(iIndex);
}

void CDynamicMesh::Play_Animation(const _float & fTimeDelta)
{
	m_pAniCtrl->Play_Animation(fTimeDelta);

	_matrix		matTemp;
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixRotationY(&matTemp, D3DXToRadian(180.f)));
}

Engine::_bool Engine::CDynamicMesh::Is_AnimationsetFinish(void)
{
	return m_pAniCtrl->Is_AnimationsetFinish();
}

const	D3DXFRAME_DERIVED* Engine::CDynamicMesh::Get_FrameByName(const char* pFrameName)
{	
	return(D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);
}

HRESULT Engine::CDynamicMesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar	szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader::Create(m_pGraphicDev, pFilePath);
	NULL_CHECK_RETURN(m_pLoader, E_FAIL);

	LPD3DXANIMATIONCONTROLLER		pAniCtrl = nullptr;

	if (FAILED(D3DXLoadMeshHierarchyFromX(szFullPath,
		D3DXMESH_MANAGED,
		m_pGraphicDev,
		m_pLoader,	// HierarchyLoader 클래스 포인터
		NULL, 
		&m_pRootFrame, 
		&pAniCtrl)))	// aniCtrl 포인터		
		return E_FAIL;

	m_pAniCtrl = CAniCtrl::Create(pAniCtrl);
	NULL_CHECK_RETURN(m_pAniCtrl, E_FAIL);
	
	Safe_Release(pAniCtrl);

	_matrix		matTemp;
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixRotationY(&matTemp, D3DXToRadian(180.f)));

	SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)m_pRootFrame);

	return S_OK;
}

	void Engine::CDynamicMesh::Render_Meshes(void)
{
	
	for (auto& iter : m_MeshcontainerList)
	{
		D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = iter;

		for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
		{
			pDerivedMeshContainer->pRenderingMatrix[i] = pDerivedMeshContainer->pFrameOffsetMatrix[i] * *pDerivedMeshContainer->ppFrameCombinedTransformMatrix[i];
		}

		void*		pSrcVtx = nullptr;
		void*		pDestVtx = nullptr;

		pDerivedMeshContainer->pOriMesh->LockVertexBuffer(0, &pSrcVtx);
		pDerivedMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDestVtx);

		// 소프트웨어 스키닝을 수행하는 함수(스키닝 애니메이션 뿐 아니라 뼈들과 정점들의 정보 변경도 수행해주는 함수)
		// 1. 뼈의 최종 변환 상태 2. 원래대로 돌리기 위한 역행렬(안넣어줘도 상관없음) 3. 변하지 않는 원본의 정점 정보 4. 애니메이션에 의해 변환된 정점 정보
		pDerivedMeshContainer->pSkinInfo->UpdateSkinnedMesh(pDerivedMeshContainer->pRenderingMatrix, NULL, pSrcVtx, pDestVtx);

		for (_ulong i = 0; i < pDerivedMeshContainer->NumMaterials; ++i)
		{
			m_pGraphicDev->SetTexture(0, pDerivedMeshContainer->ppTexture[i]);
			pDerivedMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
		
		pDerivedMeshContainer->pOriMesh->UnlockVertexBuffer();
		pDerivedMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
	}

}

// 재귀 함수
void Engine::CDynamicMesh::Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix)
{
	if (nullptr == pFrame)
		return;
	
	// 부모와 자식이 연결된 행렬의 상태 = 뼈가 지닌 원본 행렬 * 부모의 상태 행렬
	// 애니메이션과는 전혀 상관이 없는 부모와 자식의 계층 구조 형태를 유지하기 위한 상태 행렬의 갱신을 위한 용도
	// 최종적인 그리기 행렬 = pFrame->CombinedTransformMatrix  * 애니메이션 행렬
	pFrame->CombinedTransformMatrix = pFrame->TransformationMatrix * *pParentMatrix;

	// 형제 뼈의 정보가 있다면
	if (nullptr != pFrame->pFrameSibling)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, pParentMatrix);

	// 자식 뼈의 정보가 있다면
	if (nullptr != pFrame->pFrameFirstChild)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, &pFrame->CombinedTransformMatrix);
}

CDynamicMesh* Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CDynamicMesh*	pInstance = new CDynamicMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CDynamicMesh::Clone(void)
{
	return new CDynamicMesh(*this);
}

void Engine::CDynamicMesh::Free(void)
{
	if (false == m_bClone)
	{
		m_pLoader->DestroyFrame(m_pRootFrame);
		Safe_Release(m_pLoader);
		
	}
	m_MeshcontainerList.clear();

	Safe_Release(m_pAniCtrl);

	CComponent::Free();
}

void Engine::CDynamicMesh::SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
		{
			// 뼈의 이름을 얻어오는 함수
			const char*		pBoneName = pDerivedMeshContainer->pSkinInfo->GetBoneName(i);
			// 루트뼈부터 순회하면서 이름에 해당하는 뼈 정보를 찾아주는 함수
			D3DXFRAME_DERIVED*	pDerivedFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName);
			pDerivedMeshContainer->ppFrameCombinedTransformMatrix[i] = &pDerivedFrame->CombinedTransformMatrix;
		}

		m_MeshcontainerList.push_back(pDerivedMeshContainer);
	}

	if (nullptr != pFrame->pFrameSibling)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);

	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);

}

