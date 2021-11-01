#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;

END

class CHitCheckManager : public CGameObject
{

	DECLARE_SINGLETON(CHitCheckManager)
private:
	explicit CHitCheckManager(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHitCheckManager(const CHitCheckManager& rhs);
	virtual ~CHitCheckManager(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;
private:
	HRESULT		 Add_Component(void);

private:
};