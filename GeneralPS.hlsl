float4 main(float3 col : Colour) : SV_TARGET
{
	return float4(col, 1.0f);
}