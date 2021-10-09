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

	if (m_bFix)
	{
		Mouse_Fix();
		Mouse_Move(fTimeDelta);
	}


	Fallow_Target();

	_int	iExit = CCamera::Update_Object(fTimeDelta);


	return iExit;
}

void CDynamicCamera::Key_Input(const _float& fTimeDelta, const _float& fSpeed)
{
	if (m_pTarget != nullptr)
		return;
	/*_matrix		matCamWorld;
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
	}*/

	if (Get_DIKeyState(DIK_BACKSPACE) & 0x80)
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
	if (m_pTarget != nullptr)
	{
		_long	dwMouseMove = 0;

		if (dwMouseMove = Get_DIMouseMove(DIMS_Y))
		{
			m_fMouseRotation += (dwMouseMove / 10.f);
			if (m_fMouseRotation > 55.f)
				m_fMouseRotation = 55.f;
			if (m_fMouseRotation < -55.f)
				m_fMouseRotation = -55.f;

		}
		if (dwMouseMove = Get_DIMouseMove(DIMS_X)) //플레이어 자체를 회전
		{
			m_pTargetTransCom->Rotation(ROT_Y, D3DXToRadian(dwMouseMove / 10.f));
		}
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

	_vec3 vTargetLookDir = {};
	m_pTargetTransCom->Get_Info(INFO_LOOK, &vTargetLookDir);
	_vec3 TargetPos = {};
	_vec3 TargetCameraPos = {};
	m_pTargetTransCom->Get_Info(INFO_POS, &TargetPos);
	_matrix	matWorld, matInverseWorld;
	m_pTargetTransCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matInverseWorld, NULL, &matWorld);
	
	D3DXVec3TransformCoord(&TargetPos, &TargetPos, &matInverseWorld);// 타겟 좌표 로컬로 내린 후

	TargetCameraPos.x = TargetPos.x + 50.f;
	if (m_fMouseRotation > 0)
	{
		TargetCameraPos.y = TargetPos.y + 150.f * (2-cosf(D3DXToRadian(m_fMouseRotation))); // 마우스 회전값을 기준으로 거리를 전환
		//TargetCameraPos.z = TargetPos.z - (220.f *cosf(D3DXToRadian(m_fMouseRotation) - 220.f)); // 타겟 로컬 기준으로 위치를 잡아준 후
		TargetCameraPos.z = TargetPos.z - 470.f * cosf(D3DXToRadian(m_fMouseRotation)); // 타겟 로컬 기준으로 위치를 잡아준 후
	}
	else
	{
		TargetCameraPos.y = TargetPos.y + 150.f * cosf(D3DXToRadian(m_fMouseRotation)); // 마우스 회전값을 기준으로 거리를 전환
		TargetCameraPos.z = TargetPos.z - 470.f * cosf(D3DXToRadian(m_fMouseRotation)); // 타겟 로컬 기준으로 위치를 잡아준 후

	}

	
	D3DXVec3TransformCoord(&TargetCameraPos, &TargetCameraPos, &matWorld); //해당 로컬 위치를 타겟 월드 행렬로 변환

	m_vEye = TargetCameraPos;

	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	_vec3		vRight;
	memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

	_matrix		matRot;
	D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(m_fMouseRotation));
	D3DXVec3TransformNormal(&vTargetLookDir, &vTargetLookDir, &matRot);

	m_vAt.x = TargetCameraPos.x + vTargetLookDir.x*100.f;
	m_vAt.y = TargetCameraPos.y + vTargetLookDir.y*100.f;
	m_vAt.z = TargetCameraPos.z + vTargetLookDir.z*100.f;


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
