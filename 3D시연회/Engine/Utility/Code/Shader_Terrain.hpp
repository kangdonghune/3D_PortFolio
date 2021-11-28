matrix			g_matWorld;		// 상수 테이블 : 고유 이름
matrix			g_matView;
matrix			g_matProj;

texture			g_BaseTexture;

vector			g_vLightDir;

vector			g_vLightDiffuse;
vector			g_vLightAmbient;
vector			g_vLightSpecular;

vector			g_vMtrlDiffuse;
vector			g_vMtrlAmbient;
vector			g_vMtrlSpecular;

float			g_fPower;

vector			g_vCameraPos;

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
	float4		vNormal	  : NORMAL;		// 로컬
	float2		vTexUV    : TEXCOORD0; 
};

struct VS_OUT
{
	float4		vPosition : POSITION;	
	float4		vShade : COLOR0;
	float4		vSpecular : COLOR1;
	float2		vTexUV : TEXCOORD0;
	
	//float4		vColor : COLOR0;
};

// 버텍스 쉐이더
VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT	 Out = (VS_OUT)0;

	matrix	matWV, matWVP;

	matWV  = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);
	
	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	vector vWolrdNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));

	vector vWolrdDir = g_vLightDir * -1.f;

	// saturate : 함수 안의 결과 값을 0~1로 clamp하는 함수
	float	fIntensity = saturate(dot(normalize(vWolrdDir), vWolrdNormal));
	//float	fIntensity = max(dot(normalize(vWolrdDir), vWolrdNormal), 0.f);

	Out.vShade = fIntensity;
	//RGBA = FFFF
	Out.vShade.a = 1.f;

	// 정반사광
	// 반사 벡터를 구해주는  쉐이더 함수 : 빛의 방향 벡터, 법선 벡터
	vector vReflect = normalize(reflect(normalize(g_vLightDir), vWolrdDir));

	vector vWolrdPos = mul(vector(In.vPosition .xyz, 1.f), g_matWorld);

	vector vLook = vWolrdPos - g_vCameraPos;

	Out.vSpecular = pow(saturate(dot(vReflect, normalize(vLook)*-1.f)),g_fPower);
	Out.vSpecular.a = 1.f;

	return Out;
}

struct PS_IN
{
	vector		vShade		: COLOR0;
	vector		vSpecular	: COLOR1;
	float2		vTexUV		: TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor		: COLOR0;
};

// 픽셀 쉐이더
PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT	 Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV); 
	
	//Out.vColor = (In.vShade * Out.vColor) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);
	//Out.vColor = (In.vShade) * (g_vLightDiffuse * g_vMtrlDiffuse) + Out.vColor * (g_vLightAmbient * g_vMtrlAmbient);

	Out.vColor = (Out.vColor ) * (g_vLightDiffuse * g_vMtrlDiffuse) * (In.vShade + (g_vLightAmbient * g_vMtrlAmbient)) + In.vSpecular * (g_vLightSpecular * g_vMtrlSpecular);

	//Out.vColor.r = 1.f;

	return Out;
}

technique	Default_Device
{
	pass Temp
	{
	
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}	

};
