#include "stdafx.h"
#include "DynamicCamera.h"

#include "Export_Function.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{

}

CDynamicCamera::~CDynamicCamera(void)
{

}

HRESULT CDynamicCamera::Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);


	return S_OK;
}

Engine::_int CDynamicCamera::Update_Object(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta, 20.f);

	if (true == m_bFix)
	{
		//Mouse_Fix();
		//Mouse_Move(fTimeDelta);
	}	

	Fallow_Target();

	_int	iExit = CCamera::Update_Object(fTimeDelta);


	return iExit;
}

void CDynamicCamera::Key_Input(const _float& fTimeDelta, const _float& fSpeed)
{
	if (m_pTarget != nullptr)
		return;
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	if (Get_DIKeyState(DIK_W) & 0x80)
	{
		_vec3		vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * fSpeed * fTimeDelta;
		
		m_vEye += vLength;
		m_vAt	+= vLength;
	}

	if (Get_DIKeyState(DIK_S) & 0x80)
	{
		_vec3		vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * fSpeed * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}


	if (Get_DIKeyState(DIK_D) & 0x80)
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * fSpeed * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Get_DIKeyState(DIK_A) & 0x80)
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * fSpeed * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if (Get_DIKeyState(DIK_X) & 0x80)
	{
		if (true == m_bClick)
			return;

		m_bClick = true;

		if (true == m_bFix)
			m_bFix = false;
		else
			m_bFix = true;

	}
	else
		m_bClick = false;



	if (false == m_bFix)
		return;

}

void CDynamicCamera::Mouse_Move(const _float& fTimeDelta)
{
	if (m_pTarget == nullptr)
	{
		_matrix		matCamWorld;
		D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

		_long	dwMouseMove = 0;

		if (dwMouseMove = Get_DIMouseMove(DIMS_Y))
		{
			_vec3		vRight;
			memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

			_vec3		vLook = m_vAt - m_vEye;

			_matrix		matRot;
			D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

			m_vAt = m_vEye + vLook;
		}

		if (dwMouseMove = Get_DIMouseMove(DIMS_X))
		{
			_vec3		vUp = _vec3(0.f, 1.f, 0.f);

			_vec3		vLook = m_vAt - m_vEye;

			_matrix		matRot;
			D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 10.f));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

			m_vAt = m_vEye + vLook;
		}
	}

	else
	{

	}


}

void CDynamicCamera::Mouse_Fix(void)
{
	POINT		ptMouse{ (WINCX >> 1), WINCY >> 1 };
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);


}

void CDynamicCamera::Fallow_Target()
{
	
	if (m_pTarget == nullptr)
		return;

	_vec3 vTargetLookPos = {};
	_vec3 vTargetLookDir = {};
	m_pTargetTransCom->Get_Info(INFO_LOOK, &vTargetLookDir);
	_vec3 TargetCameraPos = {};
	m_pTargetTransCom->Get_Info(INFO_POS, &TargetCameraPos);

	m_vEye = TargetCameraPos;
	m_vEye.z = m_vEye.z;
	m_vEye.y = m_vEye.y + 1.f;


	vTargetLookPos = TargetCameraPos;
	vTargetLookPos.x = vTargetLookPos.x + vTargetLookDir.x*100.f;
	vTargetLookPos.y += 1.f;
	vTargetLookPos.z = vTargetLookPos.z + vTargetLookDir.z*100.f;
	//D3DXVec3TransformNormal(&vTargetLook, &vTargetLook, &m_matView);
	m_vAt = vTargetLookPos;

}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CDynamicCamera*	pInstance = new CDynamicCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
		Safe_Release(pInstance);

	return pInstance;
}

void CDynamicCamera::Free(void)
{
	CCamera::Free();
}
