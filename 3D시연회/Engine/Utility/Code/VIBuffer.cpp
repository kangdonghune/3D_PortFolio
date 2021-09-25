#include "VIBuffer.h"

USING(Engine)

Engine::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
	, m_dwVtxCnt(0)
	, m_dwVtxSize(0)
	, m_dwTriCnt(0)
	, m_dwFVF(0)
{

}

Engine::CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_dwFVF(rhs.m_dwFVF)
{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

Engine::CVIBuffer::~CVIBuffer(void)
{

}

HRESULT Engine::CVIBuffer::Ready_Buffer(void)
{
	// 버텍스 버퍼 생성
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt,	// 그리고자 하는 도형의 정점의 용량(삼각형이니 정점 세개 크기만큼의 용량)
													0,					// 정적버퍼를 사용함(정해진 배열 공간을 사용), D3DUSAGE_DYNAMIC 옵션을 주면 동적버퍼(파티클 용도)
													m_dwFVF,			// 정점의 속성 정보
													D3DPOOL_MANAGED,	// 정적 버퍼 사용시, MANAGED 동적 버퍼 사용 시, DEFAULT
													&m_pVB,
													NULL)))				// 공유할 핸들이지만 기본값 NULL				
													return E_FAIL;


	// 버텍스 버퍼 생성
	if (FAILED(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt,	// 그리고자 하는 도형의 인덱스 개수에 비례한 메모리의 용량
													0,					// 정적버퍼를 사용함(정해진 배열 공간을 사용), D3DUSAGE_DYNAMIC 옵션을 주면 동적버퍼(파티클 용도)
													m_IdxFmt,			// 인덱스의 정보	
													D3DPOOL_MANAGED,	// 정적 버퍼 사용시, MANAGED 동적 버퍼 사용 시, DEFAULT
													&m_pIB,
													NULL)))				// 공유할 핸들이지만 기본값 NULL				
													return E_FAIL;
		

	return S_OK;
}

void Engine::CVIBuffer::Render_Buffer(void)
{
	// 디바이스 장치에 내가 그리고자하는 버텍스 버퍼를 연결하는 함수
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	// 1. 몇 번 슬롯에 통해 보관, 출력할 것인가
	// 2. 버텍스들의 내용을 담고 있는 주체는 누구이며, 어떤 것을 넘겨줄 것인가
	// 3. 어디서부터 그리기를 수행할 것인가(메모리 단위이기 때문에 0byte부터)
	// 4. 어떤 크기로 그리기를 수행할 것인가

	// 현재 그리려는 버텍스의 속성을 지정하는 함수
	m_pGraphicDev->SetFVF(m_dwFVF);
	// 디바이스 장치와 인덱스 버퍼를 연결해주는 함수
	
	m_pGraphicDev->SetIndices(m_pIB);


	// 도형 그리기 함수(버텍스만을 이용한 그리기)
	// m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);
	// 1. 삼각형을 그리는 방식을 선택하는 플래그
	// 2. 몇 번째 버텍스부터 그릴 것인가
	// 3. 그리고자하는 삼각형의 개수

	// 도형 그리기 함수(인덱스 버퍼를 이용한 그리기)
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);

}

void Engine::CVIBuffer::Free(void)
{
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	CComponent::Free();
}

