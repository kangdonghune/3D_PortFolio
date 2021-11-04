#include "stdafx.h"
#include "DynamicCamera.h"
#include "Player.h"

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

	m_CameraSetting = {40.f, 150.f, 150.f, 45.f};

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
	if (!m_bMode)
	{
		Check_TargetState();
		Move_Camera(fTimeDelta);
		Fallow_Target();
	}

	else
	{
		CCTV_Mod(fTimeDelta);
	}
	
	_int	iExit = CCamera::Update_Object(fTimeDelta);


	return iExit;
}

void CDynamicCamera::Key_Input(const _float& fTimeDelta, const _float& fSpeed)
{
	if (m_pTarget != nullptr)
		return;


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
			if (m_fMouseRotation > 	m_CameraSetting.w)
				m_fMouseRotation = m_CameraSetting.w;
			if (m_fMouseRotation < -m_CameraSetting.w)
				m_fMouseRotation = -m_CameraSetting.w;

			if(m_TargetState == Player3State::AIM)
				m_pTargetTransCom->Rotation2(ROT_X, D3DXToRadian(m_fMouseRotation) );
			else
				m_pTargetTransCom->Rotation2(ROT_X, D3DXToRadian(0.f));

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

void CDynamicCamera::Check_TargetState()
{
	if (m_pTarget == nullptr)
		return;

	m_TargetState = ((CPlayer*)m_pTarget)->Get_3State();
}

void CDynamicCamera::Fallow_Target()
{
	
	if (m_pTarget == nullptr)
		return;



	_vec3 TargetPos = {};
	_vec3 TargetCameraPos = {};
	m_pTargetTransCom->Get_Info(INFO_POS, &TargetPos);
	_matrix	matWorld, matInverseWorld;
	m_pTargetTransCom->Get_WorldMatrix(&matWorld);
	D3DXMatrixInverse(&matInverseWorld, NULL, &matWorld);
	
	D3DXVec3TransformCoord(&TargetPos, &TargetPos, &matInverseWorld);// 타겟 좌표 로컬로 내린 후

	TargetCameraPos.x = TargetPos.x + m_CameraSetting.x;
	if (m_fMouseRotation > 0)
	{
		TargetCameraPos.y = TargetPos.y + m_CameraSetting.y * (2-cosf(D3DXToRadian(m_fMouseRotation))); // 마우스 회전값을 기준으로 거리를 전환
		//TargetCameraPos.z = TargetPos.z - (220.f *cosf(D3DXToRadian(m_fMouseRotation) - 220.f)); // 타겟 로컬 기준으로 위치를 잡아준 후
		TargetCameraPos.z = TargetPos.z - m_CameraSetting.z * cosf(D3DXToRadian(m_fMouseRotation)); // 타겟 로컬 기준으로 위치를 잡아준 후
	}
	else
	{
		TargetCameraPos.y = TargetPos.y + m_CameraSetting.y * cosf(D3DXToRadian(m_fMouseRotation)); // 마우스 회전값을 기준으로 거리를 전환
		TargetCameraPos.z = TargetPos.z - m_CameraSetting.z * cosf(D3DXToRadian(m_fMouseRotation)); // 타겟 로컬 기준으로 위치를 잡아준 후

	}

	
	D3DXVec3TransformCoord(&TargetCameraPos, &TargetCameraPos, &matWorld); //해당 로컬 위치를 타겟 월드 행렬로 변환

	m_vEye = TargetCameraPos;

	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	_vec3		vRight;
	memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

	_vec3 vTargetLookDir = {};
	m_pTargetTransCom->Get_Info(INFO_LOOK, &vTargetLookDir);



	if (m_TargetState == Player3State::AIM)
	{
		//_matrix		matRot;
		//D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(m_fMouseRotation));
		//D3DXVec3TransformNormal(&vTargetLookDir, &vTargetLookDir, &matRot);

	}
	else
	{
		_matrix		matRot;
		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(m_fMouseRotation));
		D3DXVec3TransformNormal(&vTargetLookDir, &vTargetLookDir, &matRot);
	}

	m_vAt.x = TargetCameraPos.x + vTargetLookDir.x;
	m_vAt.y = TargetCameraPos.y + vTargetLookDir.y;
	m_vAt.z = TargetCameraPos.z + vTargetLookDir.z;



	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(m_fFov), m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);


}

void CDynamicCamera::Move_Camera(const _float & fTimeDelta)
{
	switch (m_TargetState)
	{
	case Player3State::MOVE:
		if (m_CameraSetting.x < 40.f)
			m_CameraSetting.x += 10.f * fTimeDelta;
		if (m_CameraSetting.y > 150.f)
			m_CameraSetting.y -= 600.f * fTimeDelta;
		if (m_CameraSetting.z < 180.f)
			m_CameraSetting.z += 600.f * fTimeDelta;
		if (m_fFov <= 60.f)
			m_fFov += 40.f * fTimeDelta;

		m_CameraSetting.w = 35.f;
		break;
	case Player3State::AIM:
		if (m_CameraSetting.x > 35.f)
			m_CameraSetting.x -= 10.f * fTimeDelta;
		if (m_CameraSetting.y < 155.f)
			m_CameraSetting.y += 600.f * fTimeDelta;
		if (m_CameraSetting.z > 60.f)
			m_CameraSetting.z -= 600.f * fTimeDelta;
		if (m_fFov > 30.f)
			m_fFov -= 40.f * fTimeDelta;
		//m_CameraSetting.x = 35.f;
		//m_CameraSetting.y = 165.f;
		//m_CameraSetting.z = 60.f;
		//m_fFov = 30.f;
		m_CameraSetting.w = 15.f;
		break;
	case Player3State::IDLE:
		if (m_CameraSetting.x < 40.f)
			m_CameraSetting.x += 10.f * fTimeDelta;
		if (m_CameraSetting.y > 150.f)
			m_CameraSetting.y -= 600.f * fTimeDelta;
		if (m_CameraSetting.z < 150.f)
			m_CameraSetting.z += 600.f * fTimeDelta;
		if (m_CameraSetting.z > 150.f)
			m_CameraSetting.z -= 600.f * fTimeDelta;
		if (m_fFov < 45.f)
			m_fFov += 40.f * fTimeDelta;
		if (m_fFov > 45.f)
			m_fFov -= 40.f * fTimeDelta;
		m_CameraSetting.w = 55.f;
		break;
	case Player3State::NONE:
		break;
	default:
		break;
	}
}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CDynamicCamera*	pInstance = new CDynamicCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
		Safe_Release(pInstance);

	return pInstance;
}

void CDynamicCamera::CCTV_Mod(_float fTimeDelta)
{
	m_fCount += 2.f * fTimeDelta;
	_int iCount = m_fCount / 5.f;

	switch (iCount)
	{
	case 0:
		m_vEye = _vec3{ 70.f, 20.f, 50.f };
		m_vAt.x = m_vEye.x - 1.f;
		m_vAt.y = m_vEye.y - 1.f;
		m_vAt.z = m_vEye.z - 1.f;
		break;
	case 1:
		m_vEye = _vec3{ 45.f, 15.f, 30.f };
		m_vAt.x = m_vEye.x - 1.f;
		m_vAt.y = m_vEye.y - 1.f;
		m_vAt.z = m_vEye.z - 1.f;
		break;
	case 2:
		m_vEye = _vec3{ 80.f, 20.f, 70.f };
		m_vAt.x = m_vEye.x - 1.f;
		m_vAt.y = m_vEye.y - 1.f;
		m_vAt.z = m_vEye.z + 1.f;
		break;
	default:
		m_bMode = false;
		break;
	}
	
}

void CDynamicCamera::Free(void)
{
	CCamera::Free();
}
