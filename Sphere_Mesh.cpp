#include "Sphere_Mesh.h"

USING(Engine)

Engine::CSphrer_Mesh::CSphrer_Mesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pMesh(nullptr)
{

}

Engine::CSphrer_Mesh::CSphrer_Mesh(const CSphrer_Mesh& rhs)
	: CComponent(rhs)
	, m_pMesh(rhs.m_pMesh)
{
	m_pMesh->AddRef();
}

Engine::CSphrer_Mesh::~CSphrer_Mesh(void)
{

}

CSphrer_Mesh* Engine::CSphrer_Mesh::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSphrer_Mesh*		pInstance = new CSphrer_Mesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Mesh()))
		Safe_Release(pInstance);

	return pInstance;
}


HRESULT Engine::CSphrer_Mesh::Ready_Mesh(void)
{
	D3DXCreateSphere(m_pGraphicDev, 1, 10, 10, &m_pMesh, NULL);


	LPDIRECT3DVERTEXBUFFER9 pTempVerTexBuffer;
	m_pMesh->CloneMeshFVF(0, FVF_SPHERE, m_pGraphicDev, &m_pMesh);

	if (SUCCEEDED(m_pMesh->GetVertexBuffer(&pTempVerTexBuffer)))
	{
		int nNumVertexs = m_pMesh->GetNumVertices();
		VTXSPHERE* pVertices = NULL;

		pTempVerTexBuffer->Lock(0, 0, (void**)&pVertices, 0);

		for (int i = 0; i < nNumVertexs; i++)
		{
			pVertices[i].diffuse = D3DCOLOR_COLORVALUE(0.0f, 0.0, 1.0, 1.0f);
		}
		pTempVerTexBuffer->Unlock();
		pTempVerTexBuffer->Release();
	}



	return S_OK;
}

void Engine::CSphrer_Mesh::Render_Mesh(void)
{
	m_pMesh->DrawSubset(0);
}

void Engine::CSphrer_Mesh::Free(void)
{
	CComponent::Free();
	Safe_Release(m_pMesh);
}

CComponent* Engine::CSphrer_Mesh::Clone(void)
{
	return new CSphrer_Mesh(*this);
}
