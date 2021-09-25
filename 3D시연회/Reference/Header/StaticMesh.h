#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Component.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CStaticMesh : public CComponent
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStaticMesh(const CStaticMesh& rhs);
	virtual ~CStaticMesh(void);

public:
	const _vec3*			Get_VtxPos(void) { return m_pVtxPos; }
	const _ulong&			Get_VtxSize(void) { return m_dwVtxSize; }
	const _ulong&			Get_NumVtx(void) { return m_dwNumVtx; }

public:
	HRESULT			Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void			Render_Meshes(void);
	
private:

	LPD3DXBUFFER		m_pAdjacency;	// 인접한 폴리곤의 첫번째 주소
	LPD3DXBUFFER		m_pSubset;		// 메쉬를 구성하는 폴리곤의 집합 또는 파츠
										
										// 서브셋 개수 == 재질의 개수 == 텍스쳐의 개수
	_ulong				m_dwSubsetCnt;	// 서브셋의 개수


	D3DXMATERIAL*		m_pMtrl;		// 메쉬가 지닌 재질 정보와 텍스쳐의 이름을 저장하기 위한 구조체 포인터
	LPDIRECT3DTEXTURE9*	m_ppTexture;	// 텍스쳐의 컴객체, 동적 배열로 할당하여 사용할 예정

	_ulong				m_dwVtxSize;	// 메쉬가 지닌 정점의 크기
	_vec3*				m_pVtxPos;		// 메쉬가 지닌 정점 정보 중 포지션 정보를 저장하기 위한 포인터(동적 배열로 할당할 예정)
	_ulong				m_dwNumVtx;		// 메쉬가 지닌 정점의 개수

	LPD3DXMESH			m_pOriMesh;		// 최초의 생성된 메쉬 정보를 지니고 있는 컴객체
	LPD3DXMESH			m_pMesh;		// 노말정보가 삽입되어 편집이 끝난 메쉬 컴객체


public:
	static CStaticMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent*		Clone(void);
	virtual void Free(void);
};

END
#endif // StaticMesh_h__
