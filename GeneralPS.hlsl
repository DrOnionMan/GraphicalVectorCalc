//take the color info passed in
float4 main(float3 col : Colour) : SV_TARGET
{
	//return it with alpha value of 1
	//note cols passed in as 8 bit uints but were normallised
	//to the 0 -> 1.0f range used by HLSL
	return float4(col, 1.0f);
}