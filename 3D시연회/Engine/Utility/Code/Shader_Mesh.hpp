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
//vector			g_vCamPos;


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
	float4		vNormal : NORMAL;		// ����
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;

	//float4		vColor : COLOR0;
};

// ���ؽ� ���̴�
VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT	 Out = (VS_OUT)0;

	matrix	matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vProjPos = Out.vPosition;
	Out.vTexUV = In.vTexUV;

	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));

#pragma region �����彦�̵� ������
	//vector vWolrdDir = g_vLightDir * -1.f;

	//// saturate : �Լ� ���� ��� ���� 0~1�� clamp�ϴ� �Լ�
	//float	fIntensity = saturate(dot(normalize(vWolrdDir), vWolrdNormal));
	////float	fIntensity = max(dot(normalize(vWolrdDir), vWolrdNormal), 0.f);

	//Out.vShade = fIntensity;
	////RGBA = FFFF
	//Out.vShade.a = 1.f;

	//// ���ݻ籤
	//// �ݻ� ���͸� �����ִ� ���̴� �Լ� : 1���� ���� ���� ����, 2���� ���� ����
	//vector vReflect = normalize(reflect(normalize(g_vLightDir), vWolrdNormal));

	//vector	vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);

	//vector	vLook = vWorldPos - g_vCamPos;

	//Out.vSpecular = pow(saturate(dot(vReflect, normalize(vLook) * -1.f)), g_fPower);
	//Out.vSpecular.a = 1.f;
#pragma endregion



	return Out;
}

struct PS_IN
{
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
};

struct PS_OUT
{
	float4		vColor : COLOR0;
	float4		vNormal : COLOR1;
	float4		vDepth : COLOR2;
};

// �ȼ� ���̴�
PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT	 Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	//Out.vColor = (Out.vColor) * (g_vLightDiffuse * g_vMtrlDiffuse) * (In.vShade + (g_vLightAmbient * g_vMtrlAmbient)) +In.vSpecular * (g_vLightSpecular * g_vMtrlSpecular);
	//���� ���ͷ� ���� ������ xy�� ��ǥ ������ -1~1 ������ ���� �ش��Ѵ�
	//�츮�� ����ϰ��� �ϴ� ���� Ÿ���� �ؽ�ó�̱⿡ uv��ǥ�� �ش��ϴ� 0~1���� ������ ��ȯ����� �Ѵ�..

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w * 0.001f, 0.f, 0.f); // 0.001f = farz


	Out.vColor.a = 1.f;
	return Out;
}

PS_OUT		PS_ALPHA(PS_IN In)
{
	PS_OUT	 Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
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

	pass Temp
	{

	alphatestenable = true;
	alphafunc = greater;
	alpharef = 0xc0;
	cullmode = none;

	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_ALPHA();
	}

};
