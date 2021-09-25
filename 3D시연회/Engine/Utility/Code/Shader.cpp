#include "Shader.h"

USING(Engine)

Engine::CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pEffect(nullptr)
	, m_pErrMsg(nullptr)
{

}

Engine::CShader::CShader(const CShader& rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
{
	m_pEffect->AddRef();
}

Engine::CShader::~CShader(void)
{

}

HRESULT Engine::CShader::Ready_Shader(const _tchar* pFilePath)
{
	// 만약에 m_pEffect 값이 있고, m_pErrMsg 값이 없는 경우 : 쉐이더 코드가 정상적으로 작성되어 컴파일에 문제가 없는 경우
	// 만약에 m_pEffect 값이 없고, m_pErrMsg 값이 있는 경우 : 쉐이더 코드에 문제가 있어서 컴파일이 불가능한 경우
	// 만약에 m_pEffect 값이 있고, m_pErrMsg 값도 있는 경우 : 쉐이더 코드에 컴파일 시점에 에러가 될 만한 문제는 없지만, 경고 수준의 메시지가 발생한 상황


	if (FAILED(D3DXCreateEffectFromFile(m_pGraphicDev,
										pFilePath,
										NULL,				// 쉐이더 구동 시 사용할 매크로 파일의 주소
										NULL,				// 쉐이더 구동 시 사용할 포함 파일의 주소
										D3DXSHADER_DEBUG,	// 쉐이더를 디버깅하는 외부라이브러리를 사용할 경우를 대비하여 삽입
										NULL,				// 리소스 정보를 어디에 저장할지 묻는 인자값(null인 경우 그래픽 메모리에 저장)
										&m_pEffect,			// 생성된 쉐이더 컴객체
										&m_pErrMsg)))		// 쉐이더 코드를 컴파일 시 발생하는 에러 메시지에 대한 주소값을 저장하는 버퍼
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader_Error", MB_OK);
		return E_FAIL;
	}

	else if (nullptr != m_pErrMsg)
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader_Warning", MB_OK);
	}

	return S_OK;
}

CShader* Engine::CShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath)
{
	CShader*	pInstance = new CShader(pGraphicDev);

	if (FAILED(pInstance->Ready_Shader(pFilePath)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CShader::Clone(void)
{
	return new CShader(*this);
}

void Engine::CShader::Free(void)
{
	Safe_Release(m_pEffect);

	CComponent::Free();
}

