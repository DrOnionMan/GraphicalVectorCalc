struct VSout {
	float3 colour : Colour;
	float4 pos : SV_Position;
};




VSout main(float4 pos : Position, float3 col : Colour)
{
	VSout vs;
	vs.pos = pos;
	vs.colour = col;
	return vs;
}