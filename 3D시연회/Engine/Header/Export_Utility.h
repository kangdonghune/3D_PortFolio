#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
#include "ProtoMgr.h"
#include "Renderer.h"

#include "TriCol.h"
#include "RcCol.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "CubeTex.h"

#include "Transform.h"
#include "Texture.h"
#include "Camera.h"
#include "Calculator.h"
#include "Collider.h"
#include "DynamicCollider.h"
#include "Optimization.h"

#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "NaviMesh.h"


#include "Shader.h"


#include "LightMgr.h"

BEGIN(Engine)

// management
inline HRESULT			Create_Management(LPDIRECT3DDEVICE9& pGraphicDev, CManagement** ppManagement);
inline HRESULT			Set_Scene(CScene* pScene);
inline CScene* const 	Get_Scene();
inline _int				Update_Scene(const _float& fTimeDelta);
inline void				Render_Scene(LPDIRECT3DDEVICE9& pGraphicDev);

inline 	list<CGameObject*>		Get_List(Layer type, const _tchar* pObjTag);
inline	void					Clear_List(Layer type, const _tchar* pObjTag);
HRESULT							Add_GameObject(Layer type, const _tchar* pObjTag, CGameObject* pInstance);



// protomgr
inline HRESULT			Ready_Proto(const _tchar* pProtoTag, CComponent* pComponent);
inline CComponent*		Clone_Proto(const _tchar* pProtoTag);



// renderer
inline CRenderer*	Get_Renderer(void);
inline void			Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
inline void			Render_GameObject(LPDIRECT3DDEVICE9 pGraphicDev);
inline void			Clear_RenderGroup(void);

// light
inline HRESULT				Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);
inline const D3DLIGHT9*		Get_Light(const _ulong& iIndex = 0);

inline void				Release_Utility(void);

#include "Export_Utility.inl"

END
#endif // Export_Utility_h__
