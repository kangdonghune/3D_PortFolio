#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum WINMODE	{ MODE_FULL, MODE_WIN };

	enum BUFFERID	{ BUFFER_TRICOL, BUFFER_RCCOL, BUFFER_RCTEX, BUFFER_TERRAINTEX, BUFFER_CUBETEX, BUFFER_END };

	// DYNAMIC : 매 프레임마다 갱신을 해야할 컴포넌트, STATIC : 한 번 만들면 그대로 사용할 컴포넌트
	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

	enum INFO		{ INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum ROTATION	{ ROT_X, ROT_Y, ROT_Z, ROT_END };
	enum SCALE		{ SCALE_X, SCALE_Y, SCALE_Z, SCLAE_END};

	enum TEXTUREID	{ TYPE_NORMAL, TYPE_CUBE, TYPE_END };

	enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

	enum  MOUSEKEYSTATE	{ DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum  MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	enum COLLTYPE { COL_FALSE, COL_TRUE, COL_END };

	enum CHILD { CHILD_LT, CHILD_RT, CHILD_LB, CHILD_RB,CHILD_END };
	enum CORNER { CORNER_LT, CORNER_RT, CORNER_LB, CORNER_RB,CORNER_END	};
	enum NEIGHBOR { NEIGHBOR_LEFT, NEIGHBOR_RIGHT, NEIGHBOR_TOP, NEIGHBOR_BOTTOM,	NEIGHBOR_END };

	enum Layer { ENVIRONMENT, SPHERE, TRIGGER , GAMELOGIC, PLAYER,WEAPON ,UI_LAYER, CAMERA, EFFECT, LAYER_END};

}




#endif // Engine_Enum_h__
