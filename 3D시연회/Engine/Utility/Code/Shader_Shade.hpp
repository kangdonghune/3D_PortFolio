texture		g_NormalTexture;

sampler	NormalSampler	=	sampler_state
{
	texture = g_NormalTexture;
	
};

texture		g_DepthTexture;

sampler	DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

vector		g_vLightDir;

vector		g_vLightPos;
float		g_fRange;

vector		g_vLightDiffuse;
vector		g_vLightAmbient;

vector		g_vMtrlDiffuse = (vector)1.f;
vector		g_vMtrlAmbient = (vector)1.f;


vector		g_vCamPos;
float		g_fPower = 10.f;

matrix		g_matViewInv;
matrix		g_matProjInv;

struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
};


PS_OUT	PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	vector	vNormal = tex2D(NormalSampler, In.vTexUV);

	vNormal = vector(vNormal.xyz *2.f - 1.f, 0.f);


	Out.vShade = saturate(dot(normalize(g_vLightDir)*-1.f, vNormal)) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);
	//ºûÀÇ ¹æÇâ º¤ÅÍ * ¿ªº¤ÅÍ ³»Àû ¹ý¼±


	vector vDepth = tex2D(DepthSampler,In.vTexUV);
	float  fViewZ = vDepth.y * 1000.f;

	vector vPosition;

	vPosition.x = (In.vTexUV * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV * 2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.x * fViewZ;
	vPosition.w = fViewZ;

	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);


	vector vReflect = normalize(reflect(normalize(vector(g_vLightDir.xyz, 0.f)), vNormal));
	vector vLook = g_vCamPos - vPosition;

	Out.vSpecular = pow(saturate(dot(normalize(vLook), vReflect)), g_fPower)* 0.1f;
	Out.vSpecular.a = 0.5f;
	return Out;

}

PS_OUT	PS_DICTIONAL(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	vector	vNormal = tex2D(NormalSampler, In.vTexUV);

	vNormal = vector(vNormal.xyz *2.f - 1.f, 0.f);


	Out.vShade = saturate(dot(normalize(g_vLightDir)*-1.f, vNormal)) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);
	//ºûÀÇ ¹æÇâ º¤ÅÍ * ¿ªº¤ÅÍ ³»Àû ¹ý¼±


	vector vDepth = tex2D(DepthSampler, In.vTexUV);
	float  fViewZ = vDepth.y * 1000.f;

	vector vPosition;

	vPosition.x = (In.vTexUV * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV * 2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.x * fViewZ;
	vPosition.w = fViewZ;

	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);


	vector vReflect = normalize(reflect(normalize(vector(g_vLightDir.xyz, 0.f)), vNormal));
	vector vLook = g_vCamPos - vPosition;

	//Out.vSpecular = pow(saturate(dot(normalize(vLook), vReflect)), g_fPower * 50.f);
	//Out.vSpecular.a = 1.f;
	return Out;

}

PS_OUT	PS_POINT(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	vector	vNormal = tex2D(NormalSampler, In.vTexUV);

	vNormal = vector(vNormal.xyz *2.f - 1.f, 0.f);


	//ºûÀÇ ¹æÇâ º¤ÅÍ * ¿ªº¤ÅÍ ³»Àû ¹ý¼±


	vector vDepth = tex2D(DepthSampler, In.vTexUV);
	float  fViewZ = vDepth.y * 1000.f;

	vector vPosition;

	vPosition.x = (In.vTexUV * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV * 2.f + 1.f) * fViewZ;
	vPosition.z = vDepth.x * fViewZ;
	vPosition.w = fViewZ;

	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);

	vector vLightDir = vPosition - g_vLightPos;
	float  fDistance = length(vLightDir);

	float  fAtt = saturate((g_fRange - fDistance) / (g_fRange/4));

	Out.vShade = (saturate(dot(normalize(vector(vLightDir.xyz, 0.f))*-1.f, vNormal)) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;
	Out.vShade.a = 0.f;

	vector vReflect = normalize(reflect(normalize(vector(vLightDir.xyz, 0.f)), vNormal));
	vector vLook = g_vCamPos - vPosition;

	Out.vSpecular = (pow(saturate(dot(normalize(vLook), vReflect)), g_fPower * 50.f));
	Out.vSpecular.a = 0.f;
	return Out;

}

technique	Default_Device
{
	pass NonDictional
{
	alphablendenable = true;
	srcblend = one;
	destblend = one;

	zwriteenable = false;
	vertexshader = NULL;
	pixelshader = compile ps_3_0 PS_MAIN();
}


	pass Directional
{
	alphablendenable = true;
	srcblend = one;
	destblend = one;

	zwriteenable = false;
	vertexshader = NULL;
	pixelshader = compile ps_3_0 PS_DICTIONAL();
}

	pass Point
{
	alphablendenable = true;
	srcblend = one;
	destblend = one;

	zwriteenable = false;
	vertexshader = NULL;

	vertexshader = NULL;
	pixelshader = compile ps_3_0 PS_POINT();
}
};