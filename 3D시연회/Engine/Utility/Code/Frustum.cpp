#include "Frustum.h"
#include "QuadTree.h"
#include "GameObject.h"
#include "Transform.h"

USING(Engine)

Engine::CFrustum::CFrustum(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CFrustum::~CFrustum(void)
{

}

HRESULT Engine::CFrustum::Ready_Frustum(void)
{
	m_vPoint[0] = _vec3(-1.f, 1.f, 0.f);
	m_vPoint[1] = _vec3(1.f, 1.f, 0.f);
	m_vPoint[2] = _vec3(1.f, -1.f, 0.f);
	m_vPoint[3] = _vec3(-1.f, -1.f, 0.f);

	m_vPoint[4] = _vec3(-1.f, 1.f, 1.f);
	m_vPoint[5] = _vec3(1.f, 1.f, 1.f);
	m_vPoint[6] = _vec3(1.f, -1.f, 1.f);
	m_vPoint[7] = _vec3(-1.f, -1.f, 1.f);

	return S_OK;
}

Engine::_bool Engine::CFrustum::Isin_Frustum(const _vec3* pPos)
{
	_float	fDistance = 0.f;

	for (_ulong i = 0; i < 6; ++i)
	{
		fDistance = D3DXPlaneDotCoord(&m_Plane[i], pPos);

		if (fDistance > 0.f)
			return false;
	}

	return true;
}

Engine::_bool Engine::CFrustum::Isin_Frustum(const _vec3* pPos, const _float& fRadius)
{
	_float	fDistance = 0.f;

	for (_ulong i = 0; i < 6; ++i)
	{
		fDistance = D3DXPlaneDotCoord(&m_Plane[i], pPos);

		if (fDistance > fRadius)
			return false;
	}

	return true;
}

Engine::_bool Engine::CFrustum::Isin_FrustumForObject(const _vec3* pWorldPos, const _float& fRadius)
{
	FAILED_CHECK_RETURN(Ready_Frustum(), false);

	_matrix		matProj, matView;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXMatrixInverse(&matView, NULL, &matView); 
	
	for (_ulong i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matProj);
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matView);
	}

	// x+
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);

	// x-
	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[4], &m_vPoint[0], &m_vPoint[3]);

	// y+
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);

	// y-
	D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);

	// z+	
	D3DXPlaneFromPoints(&m_Plane[4], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);

	// z-
	D3DXPlaneFromPoints(&m_Plane[5], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);

	return Isin_Frustum(pWorldPos);
}

void CFrustum::Isin_FrustumForOBuilding(list<CGameObject*> pBuildinglist, const _float & fRadius)
{
	FAILED_CHECK_RETURN(Ready_Frustum());

	_matrix		matProj, matView;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXMatrixInverse(&matView, NULL, &matView);

	for (_ulong i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matProj);

	}
	
	m_vPoint[0] = _vec3{ m_vPoint[0].x - 15.f,m_vPoint[0].y,m_vPoint[0].z - 15.f };
	m_vPoint[1] = _vec3{ m_vPoint[1].x + 15.f,m_vPoint[1].y,m_vPoint[1].z - 15.f };
	m_vPoint[2] = _vec3{ m_vPoint[2].x + 15.f,m_vPoint[2].y,m_vPoint[2].z - 15.f };
	m_vPoint[3] = _vec3{ m_vPoint[3].x - 15.f,m_vPoint[3].y,m_vPoint[3].z - 15.f };
	for (_ulong i = 0; i < 8; ++i)
	{

		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matView);

	}

	// x+
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);

	// x-
	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[4], &m_vPoint[0], &m_vPoint[3]);

	// y+
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);

	// y-
	D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);

	// z+	
	D3DXPlaneFromPoints(&m_Plane[4], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);

	// z-
	D3DXPlaneFromPoints(&m_Plane[5], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);

	for (auto& iter : pBuildinglist)
	{
		CTransform* pTransform = dynamic_cast<CTransform*>(iter->Get_Component(L"Com_Transform", ID_DYNAMIC));
		NULL_CHECK_RETURN(pTransform);
		_vec3 WorldPos = {};
		pTransform->Get_Info(INFO_POS, &WorldPos);
		if (!Isin_Frustum(&WorldPos))
			iter->Set_IsRender(false);
		else
			iter->Set_IsRender(true);
	}

}

CFrustum* Engine::CFrustum::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFrustum*		pInstance = new CFrustum(pGraphicDev);

	if (FAILED(pInstance->Ready_Frustum()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CFrustum::Free(void)
{
	Safe_Release(m_pGraphicDev);
}

void Engine::CFrustum::Isin_FrustumForTerrain(const _vec3* pVtxPos, const _ulong& dwCntX, const _ulong& dwCntZ, INDEX32* pIndex, _ulong* pTriCnt, CQuadTree* pQuadTree)
{
	FAILED_CHECK_RETURN(Ready_Frustum(), );

	_matrix		matProj, matView;

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXMatrixInverse(&matView, NULL, &matView);

	for (_ulong i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matProj);
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matView);
	}

	// x+
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);

	// x-
	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[4], &m_vPoint[0], &m_vPoint[3]);

	// y+
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);

	// y-
	D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);

	// z+	
	D3DXPlaneFromPoints(&m_Plane[4], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);

	// z-
	D3DXPlaneFromPoints(&m_Plane[5], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);

	*pTriCnt = 0;

	if (nullptr != pQuadTree)
	{
		pQuadTree->CullingForTerrain(this, pVtxPos, pIndex, pTriCnt);
	}
	else
	{
		_bool	bIsin[3] = { false };

		_ulong	dwTriCnt = 0;

		for (_ulong i = 0; i < dwCntZ - 1; ++i)
		{
			for (_ulong j = 0; j < dwCntX - 1; ++j)
			{
				_ulong dwIndex = i * dwCntX + j;

				// 오른쪽 위
				bIsin[0] = Isin_Frustum(&pVtxPos[dwIndex + dwCntX]);
				bIsin[1] = Isin_Frustum(&pVtxPos[dwIndex + dwCntX + 1]);
				bIsin[2] = Isin_Frustum(&pVtxPos[dwIndex + 1]);

				if (true == bIsin[0] || true == bIsin[1] || true == bIsin[2])
				{
					pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
					pIndex[dwTriCnt]._1 = dwIndex + dwCntX + 1;
					pIndex[dwTriCnt]._2 = dwIndex + 1;
					++dwTriCnt;
				}

				// 왼쪽 아래
				bIsin[0] = Isin_Frustum(&pVtxPos[dwIndex + dwCntX]);
				bIsin[1] = Isin_Frustum(&pVtxPos[dwIndex + 1]);
				bIsin[2] = Isin_Frustum(&pVtxPos[dwIndex]);

				if (true == bIsin[0] || true == bIsin[1] || true == bIsin[2])
				{
					pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
					pIndex[dwTriCnt]._1 = dwIndex + 1;
					pIndex[dwTriCnt]._2 = dwIndex;
					++dwTriCnt;
				}
			}
		}
		*pTriCnt = dwTriCnt;
	}

}

