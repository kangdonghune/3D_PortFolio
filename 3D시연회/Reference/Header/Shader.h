#ifndef Shader_h__
#define Shader_h__

#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader : public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShader(const CShader& rhs);
	virtual ~CShader(void);

public:
	LPD3DXEFFECT	Get_EffectHandle(void) {	return m_pEffect;}

public:
	HRESULT			Ready_Shader(const _tchar* pFilePath);

private:
	LPD3DXEFFECT				m_pEffect; //쉐이더에서 사용하는 각종 함수를 제공하는 컴객체
	LPD3DXBUFFER				m_pErrMsg; //

public:
	static CShader*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath);
	virtual CComponent*	Clone(void);
	virtual void		Free(void);
};

END
#endif // Shader_h__
