#ifndef Engine_Define_h__
#define Engine_Define_h__

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <ctime>

#include <process.h>

#define DIRECTINPUT_VERSION		0x0800
#include <dinput.h>

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//#ifndef DBG_NEW 
//
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
//#define new DBG_NEW 
//
//#endif
#endif

#pragma warning(disable : 4251)                                   



#include "Engine_Fuction.h"
#include "Engine_Macro.h"
#include "Engine_Typedef.h"
#include "Engine_Enum.h"
#include "Engine_Struct.h"


using namespace std;
using namespace Engine;

#define PI 3.141592f
#define RADIAN 57.296f

#define UNITLAYERTAG	L"GameLogic"
#define OBJECTLAYERTAG	L"GameLogic"

#endif // Engine_Define_h__
