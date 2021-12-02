#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef	struct tagVertexColor
	{
		_vec3		vPos;			
		_ulong		dwColor;		

	}VTXCOL;

	const _ulong	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef	struct tagVertexTexture
	{
		_vec3		vPos;
		_vec3		vNormal;
		_vec2		vTexUV;	

	}VTXTEX;

	const _ulong	FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexCube
	{
		_vec3			vPos;
		_vec3			vTexUV;

	}VTXCUBE;

	const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0); // FLOAT형 3개의 크기만큼의 텍스쳐 UV를 의미하는 FVF

	typedef struct tagVertexSphere
	{
		_vec3		vPos;
		_ulong		diffuse;
	}VTXSPHERE;

	const _ulong	FVF_SPHERE = D3DFVF_XYZ | D3DFVF_DIFFUSE;


	typedef struct tagVertexScreen
	{
		_vec4	vPos;
		_vec2	vTexUV;
	}VTXSCREEN;

	const _ulong	FVF_SCREEN = D3DFVF_XYZRHW | D3DFVF_TEX1; // 윈도우 좌표 상태의 포지션

	typedef struct tagIndex16
	{
		_ushort		_0;
		_ushort		_1;
		_ushort		_2;

	}INDEX16;

	typedef struct tagIndex32
	{
		_ulong		_0;
		_ulong		_1;
		_ulong		_2;

	}INDEX32;


	typedef struct D3DXFRAME_DERIVED : public D3DXFRAME
	{
		_matrix			CombinedTransformMatrix;
			
	}D3DXFRAME_DERIVED;

	typedef struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*			ppTexture;
		
		LPD3DXMESH				pOriMesh;			// 최초 x파일을 로드했을 때 지닌 변하지 않는 원본 정점 정보를 보관하기 위한 컴객체

		_ulong					dwNumBones;			// 뼈의 개수(밑의 행렬들을 연산하여 최종적으로 출력하기 위한 상태 행렬을 만들기 위해 필요)

		_matrix*				pFrameOffsetMatrix;	// 뼈가 갖고 있는 애니메이션 정보를 사용하기 위한 행렬
		_matrix**				ppFrameCombinedTransformMatrix; // CombinedTransformMatrix들의 주소값을 보관(순회하며 위의 행렬과 곱하기를 하기 위함)하기 위한 행렬 포인터
		
		_matrix*				pRenderingMatrix;		// 최종적으로 모든 연산이 종료된 상태행렬
														// pRenderingMatrix = pFrameOffsetMatrix * (*ppFrameCombinedTransformMatrix)

	}D3DXMESHCONTAINER_DERIVED;

}



#endif // Engine_Struct_h__
