#ifndef Base_h__
#define Base_h__

// 레퍼런스 카운트(참조 카운트)

// 포인를 복사할 경우, 카운트를 올리고, 복사한 포인터를 다 사용했을 경우 카운트를 내리는 개념
// 레퍼런스 카운트가 0이 됐을 경우에만 포인터를 삭제(delete)

// 클래스 지시자(dll 프로젝트에서 현재 선언된 클래스의 코드 내용을 추출하기 위한 용도)
class _declspec(dllexport) CBase 
{
protected:
	inline explicit CBase(void);
	inline virtual ~CBase(void);

public:
	inline	unsigned long	AddRef(void);
	inline	unsigned long	Release(void);

private:
	unsigned long			m_dwRefCnt;

public:
	inline virtual void Free(void) = 0; // 순수 가상함수
};

#include "Base.inl"

#endif // Base_h__


// 전역변수  : 외부에서도 접근가능한 정적 변수  (접근 범위 : 파일 하나의 내부)
// 정적변수(static) : 내부에서만 접근가능한 정적 변수(접근 범위 : 클래스, 함수 내부)



