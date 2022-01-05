cbuffer CBuf
{
	matrix transform;
};

struct VSOut
{
	float2 tex : TexCoords;
	float4 pos : SV_POSITION;
};

VSOut main(float3 pos : Position, float2 tex : TexCoords)
{
	VSOut vsout;
	vsout.tex = tex;
	vsout.pos = mul(float4(pos, 1.0f), transform);
	return vsout;
}