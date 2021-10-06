#include "NaviMesh.h"


USING(Engine)

Engine::CNaviMesh::CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_dwIndex(0)	
{

}

Engine::CNaviMesh::CNaviMesh(const CNaviMesh& rhs)
	:CComponent(rhs)
	, m_dwIndex(rhs.m_dwIndex)
	, m_vecCell(rhs.m_vecCell)
{
	for (auto& iter : m_vecCell)
		iter->AddRef();
}

Engine::CNaviMesh::~CNaviMesh(void)
{

}

HRESULT Engine::CNaviMesh::Ready_NaviMesh(void)
{
	//m_vecCell.reserve(m_vecCell.capacity());

	//CCell*	pCell = nullptr;

	//// 0
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(0.f, 0.f, 2.f), &_vec3(2.f, 0.f, 0.f), &_vec3(0.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//// 1
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(0.f, 0.f, 2.f), &_vec3(2.f, 0.f, 2.f), &_vec3(2.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//// 2
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(0.f, 0.f, 4.f), &_vec3(2.f, 0.f, 2.f), &_vec3(0.f, 0.f, 2.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//// 3
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(2.f, 0.f, 2.f), &_vec3(4.f, 0.f, 0.f), &_vec3(2.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);

	//FAILED_CHECK_RETURN(Link_Cell(), E_FAIL);

	return S_OK;
}

void Engine::CNaviMesh::Render_NaviMesh(void)
{
	for (auto& iter : m_vecCell)
	{
		if (iter->Get_Dead())
			continue;
		iter->Render_Cell();
	}
}


CNaviMesh* Engine::CNaviMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNaviMesh*	pInstance = new CNaviMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_NaviMesh()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CNaviMesh::Clone(void)
{
	return new CNaviMesh(*this);
}

void Engine::CNaviMesh::Free(void)
{
	for_each(m_vecCell.begin(), m_vecCell.end(), CDeleteObj());
	m_vecCell.clear();

	CComponent::Free();
}

HRESULT Engine::CNaviMesh::Link_Cell(void)
{

	for (_ulong i = 0; i < m_vecCell.size(); ++i)
	{
		for (_ulong j = 0; j < m_vecCell.size(); ++j)
		{
			if (i == j)
				continue;

			// 1. 이웃하는 셀 정보가 없어서 내가 연결해주고자 하는 셀을 넣을 수 있는지
			// 2. 이웃하는 셀과 기본 셀의 정점 두 개가 같은 점인지

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_AB) &&
				true    == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_A),
														m_vecCell[i]->Get_Point(CCell::POINT_B), 
														m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_AB, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_BC) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_B),
													m_vecCell[i]->Get_Point(CCell::POINT_C),
													m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_BC, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_CA) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_C),
													m_vecCell[i]->Get_Point(CCell::POINT_A),
													m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_CA, m_vecCell[j]);
				continue;
			}
		}
	}
	
	return S_OK;
}

HRESULT CNaviMesh::Add_Cell(CCell * pCell)
{

	m_vecCell.push_back(pCell);
	m_vecCell.reserve(m_vecCell.capacity());
	FAILED_CHECK_RETURN(Link_Cell(), E_FAIL);
	return S_OK;
}

HRESULT CNaviMesh::Delete_Cell()
{
	for (_ulong i = 0; i < m_vecCell.size(); ++i)
	{
		if (m_vecCell[i]->Get_Dead())
		{
			Safe_Release(m_vecCell[i]);
			m_vecCell.erase(m_vecCell.begin() + i);
			i--;
			continue;
		}

	}
	Link_Cell();
	return S_OK;
}

Engine::_vec3 Engine::CNaviMesh::Move_OnNaviMesh(const _vec3* pTargetPos,
												const _vec3* pTargetDir)
{

	_vec3	vEndPos = *pTargetPos + *pTargetDir;
	
	if (CCell::COMPARE_MOVE == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
	{
		D3DXPLANE		Plane;

		D3DXPlaneFromPoints(&Plane, m_vecCell[m_dwIndex]->Get_Point(CCell::POINT_A),
									m_vecCell[m_dwIndex]->Get_Point(CCell::POINT_B),
									m_vecCell[m_dwIndex]->Get_Point(CCell::POINT_C));
		// ax + by + cz + d = 0->y = ?
		// by = -ax - cz - d
		// y =(-ax - cz - d) / b

		vEndPos.y = ((-Plane.a * vEndPos.x - Plane.c * vEndPos.z - Plane.d) / Plane.b);
		return vEndPos;
	}

	else if (CCell::COMPARE_STOP == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
		return *pTargetPos;

}