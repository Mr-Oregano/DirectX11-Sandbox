
Texture2D tex;
SamplerState splr;

float4 main(float2 coords : TexCoords) : SV_TARGET
{
	return tex.Sample(splr, coords);
}