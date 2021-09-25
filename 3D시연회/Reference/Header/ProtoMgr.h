#ifndef ProtoMgr_h__
#define ProtoMgr_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CProtoMgr : public CBase
{
	DECLARE_SINGLETON(CProtoMgr)

private:
	explicit CProtoMgr(void);
	virtual ~CProtoMgr(void);

public:
	HRESULT			Ready_Proto(const _tchar* pProtoTag, CComponent* pComponent);
	CComponent*		Clone_Proto(const _tchar* pProtoTag);

private:
	CComponent*		Find_Proto(const _tchar* pProtoTag);

private:
	map<const _tchar*, CComponent*>		m_mapProto;

public:
	virtual void Free();
};

END
#endif // ProtoMgr_h__
