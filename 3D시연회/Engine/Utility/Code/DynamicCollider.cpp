#include "DynamicCollider.h"

USING(Engine)

Engine::CDynamicCollider::CDynamicCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CDynamicCollider::CDynamicCollider(const CDynamicCollider& rhs)
	: CVIBuffer(rhs)
	, m_fHeight(rhs.m_fHeight)
	, m_fWidthX(rhs.m_fWidthX)
	, m_fWidthY(rhs.m_fWidthY)
	, m_vMin(rhs.m_vMin)
	, m_vMax(rhs.m_vMax)

{

}

Engine::CDynamicCollider::~CDynamicCollider(void)
{

}

CDynamicCollider* Engine::CDynamicCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _float fHeight, _float fWidthX, _float fWidthZ)
{
	CDynamicCollider*		pInstance = new CDynamicCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(pPos, fHeight, fWidthX, fWidthZ)))
		Safe_Release(pInstance);

	return pInstance;
}


HRESULT Engine::CDynamicCollider::Ready_Buffer(_vec3* pPos, _float fHeight, _float fWidthX, _float fWidthZ)
{
	m_dwTriCnt = 12;
	m_dwFVF = FVF_CUBE;
	m_dwVtxCnt = 8;
	m_dwVtxSize = sizeof(VTXCUBE);

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);


	VTXCUBE*			pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);


	pVertex[0].vPos = _vec3(pPos->x - fWidthX, pPos->y + fHeight, pPos->z - fWidthZ);
	pVertex[0].vTexUV = pVertex[0].vPos;

	pVertex[1].vPos = _vec3(pPos->x + fWidthX, pPos->y + fHeight, pPos->z - fWidthZ);
	pVertex[1].vTexUV = pVertex[1].vPos;

	pVertex[2].vPos = _vec3(pPos->x - fWidthX, pPos->y, pPos->z - fWidthZ);
	pVertex[2].vTexUV = pVertex[2].vPos;

	pVertex[3].vPos = _vec3(pPos->x + fWidthX, pPos->y, pPos->z - fWidthZ);
	pVertex[3].vTexUV = pVertex[3].vPos;


	pVertex[4].vPos = _vec3(pPos->x - fWidthX, pPos->y + fHeight, pPos->z + fWidthZ);
	pVertex[4].vTexUV = pVertex[4].vPos;

	pVertex[5].vPos = _vec3(pPos->x + fWidthX, pPos->y + fHeight, pPos->z + fWidthZ); 
	pVertex[5].vTexUV = pVertex[5].vPos;

	pVertex[6].vPos = _vec3(pPos->x - fWidthX, pPos->y, pPos->z + fWidthZ);
	pVertex[6].vTexUV = pVertex[6].vPos;

	pVertex[7].vPos = _vec3(pPos->x + fWidthX, pPos->y, pPos->z + fWidthZ);
	pVertex[7].vTexUV = pVertex[7].vPos;


	m_pVB->Unlock();


	INDEX16*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// X+
	pIndex[0]._0 = 1;
	pIndex[0]._1 = 7;
	pIndex[0]._2 = 3;

	pIndex[1]._0 = 1;
	pIndex[1]._1 = 5;
	pIndex[1]._2 = 7;

	// X-
	pIndex[2]._0 = 0;
	pIndex[2]._1 = 6;
	pIndex[2]._2 = 2;

	pIndex[3]._0 = 0;
	pIndex[3]._1 = 4;
	pIndex[3]._2 = 6;

	// Y+
	pIndex[4]._0 = 4;
	pIndex[4]._1 = 1;
	pIndex[4]._2 = 0;

	pIndex[5]._0 = 4;
	pIndex[5]._1 = 5;
	pIndex[5]._2 = 1;

	// Y-
	pIndex[6]._0 = 6;
	pIndex[6]._1 = 3;
	pIndex[6]._2 = 2;

	pIndex[7]._0 = 6;
	pIndex[7]._1 = 7;
	pIndex[7]._2 = 4;

	// Z+
	pIndex[8]._0 = 0;
	pIndex[8]._1 = 3;
	pIndex[8]._2 = 2;

	pIndex[9]._0 = 0;
	pIndex[9]._1 = 1;
	pIndex[9]._2 = 3;

	// Z-
	pIndex[10]._0 = 4;
	pIndex[10]._1 = 7;
	pIndex[10]._2 = 6;

	pIndex[11]._0 = 4;
	pIndex[11]._1 = 5;
	pIndex[11]._2 = 7;


	m_pIB->Unlock();

	m_vMin = _vec3{ pPos->x - fWidthX, pPos->y, pPos->z - fWidthZ };
	m_vMax = _vec3{ pPos->x + fWidthX, pPos->y + fHeight, pPos->z + fWidthZ };
	m_vCenter = (m_vMax - m_vMin) / 2;
	m_vMaxDir = m_vMax - m_vCenter;
}


void Engine::CDynamicCollider::Render_Buffer(const _matrix* matWorld)
{
	m_matCollWorld = *matWorld;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, matWorld);

	if (GetAsyncKeyState(VK_SPACE) & 0x8001)
	{
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	CVIBuffer::Render_Buffer();


	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}

void Engine::CDynamicCollider::Free(void)
{
	CVIBuffer::Free();
}

CComponent* Engine::CDynamicCollider::Clone(void)
{
	return new CDynamicCollider(*this);
}
