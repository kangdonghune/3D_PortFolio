#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Component.h"
#include "HierarchyLoader.h"
#include "AniCtrl.h"

BEGIN(Engine)

class ENGINE_DLL CDynamicMesh : public CComponent
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicMesh(const CDynamicMesh& rhs);
	virtual ~CDynamicMesh(void);

public:
	void			Set_AnimationIndex(const _uint& iIndex);	
	void			Set_AnimationIndexRepeat(const _uint& iIndex);
	void			Play_Animation(const _float& fTimeDelta);	
	const	D3DXFRAME_DERIVED*		Get_FrameByName(const char* pFrameName);
	_bool			Is_AnimationsetFinish(void);
public:
	HRESULT		Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void		Render_Meshes(void);
	void		Render_Meshes(LPD3DXEFFECT& pEffect);

private:
	// 매 프레임마다 모든 뼈들을 순회하면서 뼈들이 가지고 있는 행렬을 변환하기 위한 함수
	void		Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix);
	void		SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame);


private:
	D3DXFRAME*								m_pRootFrame;
	CHierarchyLoader*						m_pLoader = nullptr;
	list<D3DXMESHCONTAINER_DERIVED*>		m_MeshcontainerList;

	CAniCtrl*								m_pAniCtrl;

	
public:
	static CDynamicMesh*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent*		Clone(void);
	virtual void			Free(void);	
};

END
#endif // DynamicMesh_h__

// 메쉬를 이루는 뼈 정보를 저장하기 위한 구조체
//typedef struct _D3DXFRAME
//{
//	LPSTR                   Name;			// 뼈의 이름을 저장하기 위한 문자열 포인터(멀티 바이트 기반)
//	D3DXMATRIX              TransformationMatrix;	// 최초 로딩 시점의 뼈의 행렬 상태
//
//	LPD3DXMESHCONTAINER     pMeshContainer;		// 스태틱 메쉬 정보
//
//	struct _D3DXFRAME       *pFrameSibling;		// 형제 뼈의 정보를 저장하기 위한 포인터
//	struct _D3DXFRAME       *pFrameFirstChild;	// 자식 뼈의 정보를 저장하기 위한 포인터
//} D3DXFRAME, *LPD3DXFRAME;
//
//// 메쉬의 각종 정보를 담고 있는 구성 구조체(스태틱 메쉬)
//typedef struct _D3DXMESHCONTAINER
//{
//	LPSTR                   Name;		// 메쉬 컨테이너의 이름을 저장하기 위한 문자열 포인터(거의 사용하지 않거나 없어도 무방)
//
//	D3DXMESHDATA            MeshData;	// 메쉬의 종류를 결정하고 메쉬 정보를 담기 위한 컴객체를 소유한 구조체
//
//	LPD3DXMATERIAL          pMaterials;	// 스태틱 메쉬에서 D3DXMATERIAL* 와 같은 녀석(재질정보 + 텍스쳐의 이름)
//	LPD3DXEFFECTINSTANCE    pEffects;	// 메쉬가 지닌 고유의 이펙트 정보를 담고 있는 컴객체(우리는 사용할 수 없다)
//	DWORD                   NumMaterials;	// 재질의 개수 == 서브셋의 개수 == 텍스쳐의 개수
//	DWORD                  *pAdjacency;		// 인접한 폴리곤의 정보를 순회하기 위해 첫번째 주소를 보관하기 위한 포인터
//
//	LPD3DXSKININFO          pSkinInfo;		// 스키닝 애니메이션을 구동하기 위한 각종 기능을 제공하는 컴객체
//
//	struct _D3DXMESHCONTAINER *pNextMeshContainer;		// 다음 메쉬컨테이너의 주소값을 저장하기 위한 포인터
//
//} D3DXMESHCONTAINER, *LPD3DXMESHCONTAINER;