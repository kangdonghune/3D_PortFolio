matrix			g_matWorld;		// ��� ���̺� : ���� �̸�
matrix			g_matView;
matrix			g_matProj;

texture			g_BaseTexture;

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
	float3		vTexUV    : TEXCOORD0; //0~8 ������ ��� ������ �� dx9
};

struct VS_OUT //���̴��� �� ���� �ڷ����� ���� ���� �ڷ����� �ٲ� ���� �ִ�.
{
	float4		vPosition : POSITION;	
	float3		vTexUV : TEXCOORD0;
	
	//float4		vColor : COLOR0;
};

// ���ؽ� ���̴�
VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT	 Out = (VS_OUT)0; //0���� �ʱ�ȭ �ض� 

	matrix	matWV, matWVP;

	matWV  = mul(g_matWorld, g_matView); //���ϱ� �Լ�
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
	float4		vColor		: COLOR0; //color�� 0~3 ���� 4���� ��� ����  
};

// �ȼ� ���̴�
PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT	 Out = (PS_OUT)0;

	Out.vColor = texCUBE(BaseSampler, In.vTexUV); // 2���� ������ �ؽ�ó�κ��� uv��ǥ�� �ش��ϴ� ���� �����ϴ� �Լ�(��ȯ Ÿ���� vectorŸ��)
	
	return Out;
}

// �ȼ� ���̴�
PS_OUT		PS_SOUR(PS_IN In)
{
	PS_OUT	 Out = (PS_OUT)0;

	Out.vColor = texCUBE(BaseSampler, In.vTexUV); // 2���� ������ �ؽ�ó�κ��� uv��ǥ�� �ش��ϴ� ���� �����ϴ� �Լ�(��ȯ Ÿ���� vectorŸ��)

	Out.vColor.a = 1.f;

	return Out;
}

technique	Default_Device
{
	// ����� ĸ��ȭ
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
