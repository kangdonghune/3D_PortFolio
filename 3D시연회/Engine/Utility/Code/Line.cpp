#include "Line.h"

USING(Engine)

Engine::CLine::CLine(void)
{

}

Engine::CLine::~CLine(void)
{

}

HRESULT Engine::CLine::Ready_Line(const _vec2* pPointA, const _vec2* pPointB)
{
	m_vPoint[POINT_START] = *pPointA;
	m_vPoint[POINT_FINISH] = *pPointB;

	m_vDirection = m_vPoint[POINT_FINISH] - m_vPoint[POINT_START];
	m_vNormal = _vec2(m_vDirection.y *-1.f, m_vDirection.x); //2차원 선분이라는 점을 고려해서 x와 y의 값을 바꿔주고 윈도우 좌표계이기에 y값에 -1 곱하기
	D3DXVec2Normalize(&m_vNormal, &m_vNormal);

	return S_OK;
}

Engine::CLine::COMPARE Engine::CLine::Compare(const _vec2* pEndPos)
{
	_vec2		vDir = *pEndPos - m_vPoint[POINT_START]; //시작점에서 타겟위치로의 방향 벡터

	_float		fDot = D3DXVec2Dot(&m_vNormal, D3DXVec2Normalize(&vDir, &vDir)); //선분의 법선과 타겟방향 벡터를 내적해서 예각이면 밖에 둔각이면 안쪽에 있다 판별

	if (0.f <= fDot) //양수면 예각
		return COMPARE_OUT;

	else // 음수면 둔각
		return COMPARE_IN;
}

CLine* Engine::CLine::Create(const _vec2* pPointA, const _vec2* pPointB)
{
	CLine*		pInstance = new CLine;

	if (FAILED(pInstance->Ready_Line(pPointA, pPointB)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CLine::Free(void)
{

}

