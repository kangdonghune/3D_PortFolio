texture		g_NormalTexture;

sampler	NormalSampler	=	sampler_state
{
	texture = g_NormalTexture;
	
};

vector		g_vLightDir;

vector		g_vLightDiffuse;
vector		g_vLightAmbient;

vector		g_vMtrlDiffuse = (vector)1.f;
vector		g_vMtrlAmbient = (vector)1.f;

struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vShade : COLOR0;
};


PS_OUT	PS_MAIN(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	vector	vNormal = tex2D(NormalSampler, In.vTexUV);

	vNormal = vector(vNormal.xyz *2.f - 1.f, 0.f);


	Out.vShade = saturate(dot(normalize(g_vLightDir)*-1.f, vNormal)) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);
	//ºûÀÇ ¹æÇâ º¤ÅÍ * ¿ªº¤ÅÍ ³»Àû ¹ý¼±


	return Out;

}

technique	Default_Device
{
	pass Directional
{
	zwriteenable = false;
	vertexshader = NULL;
	pixelshader = compile ps_3_0 PS_MAIN();
}
};