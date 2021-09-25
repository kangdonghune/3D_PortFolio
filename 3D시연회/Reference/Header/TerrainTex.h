#ifndef TerrainTex_h__
#define TerrainTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex : public CVIBuffer
{
private:
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex(void);

public:
	const _vec3*			Get_VtxPos(void) const { return m_pPos; }
	
	// 이 함수는 높이맵의 해상도와 정점의 개수가 일치하다는 전제 하에 사용하는 코드
	_ulong			Get_VtxCntX(void) const { return m_iH.biWidth; }
	_ulong			Get_VtxCntZ(void) const { return m_iH.biHeight; }
	
	void			Copy_Index(INDEX32* pIndex, const _ulong& dwTriCnt);
public:
	HRESULT Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	virtual void Render_Buffer(void) override;

private:
	HANDLE					m_hFile;
	BITMAPFILEHEADER		m_fH;
	BITMAPINFOHEADER		m_iH;
	_vec3*					m_pPos = nullptr;

public:
	static CTerrainTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv = 1);
	virtual CComponent*	Clone(void);
private:
	virtual void Free(void) override;

};

END
#endif // TerrainTex_h__
