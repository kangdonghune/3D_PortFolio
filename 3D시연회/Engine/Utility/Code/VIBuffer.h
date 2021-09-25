#ifndef VIBuffer_h__
#define VIBuffer_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer : public CComponent 
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer(void);

public:
	const _ulong&		Get_TriCnt(void) { return m_dwTriCnt; }
		 
public:
	virtual HRESULT		Ready_Buffer(void);
	virtual void		Render_Buffer(void);

protected:
	LPDIRECT3DVERTEXBUFFER9			m_pVB;	// 정점들을 보관하는 컴 객체
	LPDIRECT3DINDEXBUFFER9			m_pIB;	// 정점을 그리기 위한 순서를 보관하는 컴 객체	

	_ulong							m_dwTriCnt;	// 삼각형 개수
	_ulong							m_dwVtxSize; // 버텍스 크기
	_ulong							m_dwVtxCnt;	// 버텍스 개수
	_ulong							m_dwFVF;	// FVF의종류

	_ulong							m_dwIdxSize;
	D3DFORMAT						m_IdxFmt;

public:
	virtual void		Free(void);
};

END
#endif // VIBuffer_h__
