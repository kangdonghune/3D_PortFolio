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
	// ���࿡ m_pEffect ���� �ְ�, m_pErrMsg ���� ���� ��� : ���̴� �ڵ尡 ���������� �ۼ��Ǿ� �����Ͽ� ������ ���� ���
	// ���࿡ m_pEffect ���� ����, m_pErrMsg ���� �ִ� ��� : ���̴� �ڵ忡 ������ �־ �������� �Ұ����� ���
	// ���࿡ m_pEffect ���� �ְ�, m_pErrMsg ���� �ִ� ��� : ���̴� �ڵ忡 ������ ������ ������ �� ���� ������ ������, ��� ������ �޽����� �߻��� ��Ȳ


	if (FAILED(D3DXCreateEffectFromFile(m_pGraphicDev,
										pFilePath,
										NULL,				// ���̴� ���� �� ����� ��ũ�� ������ �ּ�
										NULL,				// ���̴� ���� �� ����� ���� ������ �ּ�
										D3DXSHADER_DEBUG,	// ���̴��� ������ϴ� �ܺζ��̺귯���� ����� ��츦 ����Ͽ� ����
										NULL,				// ���ҽ� ������ ��� �������� ���� ���ڰ�(null�� ��� �׷��� �޸𸮿� ����)
										&m_pEffect,			// ������ ���̴� �İ�ü
										&m_pErrMsg)))		// ���̴� �ڵ带 ������ �� �߻��ϴ� ���� �޽����� ���� �ּҰ��� �����ϴ� ����
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

