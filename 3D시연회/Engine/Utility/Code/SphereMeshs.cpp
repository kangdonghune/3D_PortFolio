#include "SphereMeshs.h"
#include "GameObject.h"
#include "Transform.h"
USING(Engine)

Engine::CSphrerMeshs::CSphrerMeshs(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pMesh(nullptr)
{

}

Engine::CSphrerMeshs::CSphrerMeshs(const CSphrerMeshs& rhs)
	: CComponent(rhs)
	, m_pMesh(rhs.m_pMesh)
{
	m_pMesh->AddRef();


}

Engine::CSphrerMeshs::~CSphrerMeshs(void)
{

}

CSphrerMeshs* Engine::CSphrerMeshs::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSphrerMeshs*		pInstance = new CSphrerMeshs(pGraphicDev);

	if (FAILED(pInstance->Ready_Mesh()))
		Safe_Release(pInstance);

	return pInstance;
}


HRESULT Engine::CSphrerMeshs::Ready_Mesh(void)
{

	////备积己
	LPD3DXMESH            pTempMesh;
	D3DXCreateSphere(m_pGraphicDev, m_iRadius, 10, 10, &pTempMesh, nullptr);
	pTempMesh->CloneMeshFVF(0, FVF_COL, m_pGraphicDev, &m_pMesh);
	pTempMesh->Release();

	m_pMesh->GetVertexBuffer(&m_pVB);
	int iNumVertex = m_pMesh->GetNumVertices();

	VTXCOL* pVert;
	m_pVB->Lock(0, 0, (void**)&pVert, 0);
	for (_int i = 0; i < iNumVertex; i++)
	{
		pVert[i].dwColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	}
	m_pVB->Unlock();
	m_pVB->Release();

	return S_OK;
}

void Engine::CSphrerMeshs::Render_Mesh()
{
	if (m_pTransCom == nullptr)
		return;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrix());

	

	////备 免仿
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pMesh->DrawSubset(0);

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}



void Engine::CSphrerMeshs::Free(void)
{
	CComponent::Free();
	Safe_Release(m_pMesh);
	Safe_Release(m_pVB);

}

CComponent* Engine::CSphrerMeshs::Clone(void)
{
	return new CSphrerMeshs(*this);
}
