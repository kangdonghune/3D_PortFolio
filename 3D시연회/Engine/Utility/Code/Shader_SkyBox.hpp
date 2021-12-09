matrix			g_matWorld;		// 상수 테이블 : 고유 이름
matrix			g_matView;
matrix			g_matProj;

texture			g_BaseTexture;

// 텍스처의 각종 성질을 지정하기 위한 구조체 
sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;	
};


struct VS_IN
{
	float4		vPosition : POSITION;	// 시멘틱 : 고유 옵션(이해하기 쉽게 표현하면 FVF)
	float3		vTexUV    : TEXCOORD0; //0~8 번까지 사용 가능한 게 dx9
};

struct VS_OUT //쉐이더는 들어갈 때의 자료형과 나올 때의 자료형이 바뀔 수도 있다.
{
	float4		vPosition : POSITION;	
	float3		vTexUV : TEXCOORD0;
	
	//float4		vColor : COLOR0;
};

// 버텍스 쉐이더
VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT	 Out = (VS_OUT)0; //0으로 초기화 해라 

	matrix	matWV, matWVP;

	matWV  = mul(g_matWorld, g_matView); //곱하기 함수
	matWVP = mul(matWV, g_matProj);
	
	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float3		vTexUV		: TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor		: COLOR0; //color는 0~3 까지 4개만 사용 가능  
};

// 픽셀 쉐이더
PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT	 Out = (PS_OUT)0;

	Out.vColor = texCUBE(BaseSampler, In.vTexUV); // 2차원 상태의 텍스처로부터 uv좌표에 해당하는 색을 추출하는 함수(반환 타입이 vector타입)
	
	return Out;
}

// 픽셀 쉐이더
PS_OUT		PS_SOUR(PS_IN In)
{
	PS_OUT	 Out = (PS_OUT)0;

	Out.vColor = texCUBE(BaseSampler, In.vTexUV); // 2차원 상태의 텍스처로부터 uv좌표에 해당하는 색을 추출하는 함수(반환 타입이 vector타입)

	Out.vColor.a = 1.f;

	return Out;
}

technique	Default_Device
{
	// 기능의 캡슐화
	pass Temp
	{
		zwriteenable = false;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	/*pass Sour
	{
		zwriteenable = false;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_SOUR();
	}*/

};
