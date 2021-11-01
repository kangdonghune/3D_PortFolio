#ifndef Logo_h__
#define Logo_h__

#include "Scene.h"
#include "Define.h"
#include "BackGround.h"
#include "Player.h"
#include "Monster.h"

#include "Loading.h"

class CLogo : public CScene
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogo(void);

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene(void) override;

private:
	HRESULT			Ready_Environment_Layer();
	HRESULT			Ready_GameLogic_Layer();
	HRESULT			Ready_UI_Layer();
	HRESULT			Ready_Camera_Layer();
	HRESULT			Ready_Weapon_Layer();
	HRESULT			Ready_Sphere_Layer();
	HRESULT			Ready_Effect_Layer();
	HRESULT			Ready_Trigger_Layer();
	HRESULT			Ready_Resource(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CLoading*			m_pLoading = nullptr;

public:
	static CLogo*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);


};

#endif // Logo_h__
