matrix			g_matWorld;		// ��� ���̺� : ���� �̸�
matrix			g_matView;
matrix			g_matProj;

texture			g_BaseTexture;
//
//vector			g_vLightDir;
//
//vector			g_vLightDiffuse;
//vector			g_vLightAmbient;
//vector			g_vLightSpecular;
//
//vector			g_vMtrlDiffuse;
//vector			g_vMtrlAmbient;
//vector			g_vMtrlSpecular;
//
//float			g_fPower;
//
//vector			g_vCameraPos;

// �ؽ�ó�� ���� ������ �����ϱ� ���� ����ü 
sampler BaseSampler = sampler_state 
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;	
};


struct VS_IN
{
	float4		vPosition : POSITION;	// �ø�ƽ : ���� �ɼ�(�����ϱ� ���� ǥ���ϸ� FVF)
	float4		vNormal	  : NORMAL;		// ����
	float2		vTexUV    : TEXCOORD0; 
};

struct VS_OUT
{
	float4		vPosition : POSITION;	
	float4		vNormal	 : NORMAL;
	float2		vTexUV	: TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
	
	//float4		vColor : COLOR0;
};

// ���ؽ� ���̴�
VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT	 Out = (VS_OUT)0;

	matrix	matWV, matWVP;

	matWV  = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);
	
	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vProjPos = Out.vPosition;

	Out.vTexUV = In.vTexUV;

#pragma region ����彦�̴� ������
	//vector vWolrdNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));

	//vector vWolrdDir = g_vLightDir * -1.f;

	//// saturate : �Լ� ���� ��� ���� 0~1�� clamp�ϴ� �Լ�
	//float	fIntensity = saturate(dot(normalize(vWolrdDir), vWolrdNormal));
	////float	fIntensity = max(dot(normalize(vWolrdDir), vWolrdNormal), 0.f);

	//Out.vShade = fIntensity;
	////RGBA = FFFF
	//Out.vShade.a = 1.f;

	//// ���ݻ籤
	//// �ݻ� ���͸� �����ִ�  ���̴� �Լ� : ���� ���� ����, ���� ����
	//vector vReflect = normalize(reflect(normalize(g_vLightDir), vWolrdDir));

	//vector vWolrdPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);

	//vector vLook = vWolrdPos - g_vCameraPos;

	//Out.vSpecular = pow(saturate(dot(vReflect, normalize(vLook)*-1.f)), g_fPower);
	//Out.vSpecular.a = 1.f;
#pragma endregion

	Out.vNormal = normalize(mul(vector(In.vPosition.xyz, 1.f), matWVP));

	return Out;
}

struct PS_IN
{
	vector		vNormal : NORMAL;
	float2		vTexUV		: TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
};

struct PS_OUT
{
	float4		vColor		: COLOR0;
	vector		vNormal : COLOR1;
	float4		vDepth : COLOR2;
};

// �ȼ� ���̴�
PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT	 Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV); 
	
	//Out.vColor = (In.vShade * Out.vColor) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);
	//Out.vColor = (In.vShade) * (g_vLightDiffuse * g_vMtrlDiffuse) + Out.vColor * (g_vLightAmbient * g_vMtrlAmbient);
	//Out.vColor = (Out.vColor ) * (g_vLightDiffuse * g_vMtrlDiffuse) * (In.vShade + (g_vLightAmbient * g_vMtrlAmbient)) + In.vSpecular * (g_vLightSpecular * g_vMtrlSpecular);

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	//r�� ��ġ�� z�����Ⱑ ���� ���� ��ǥ
	//g�� ��ġ�� �佺���̽��� z��
	//Out.vDepth = vector(In.vProjPos.z/ In.vProjPos.w, In.vProjPos.w * 0.001f,0.f, 0.f); // 0.001f = farz

	Out.vColor.a = 1.f;
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
