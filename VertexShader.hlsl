
struct VSout {
	float3 colour : Colour;
	float4 pos : SV_Position;
};

cbuffer transform {
	float4x4 transform;
};


VSout main(float4 pos : Position, float3 col : Colour)
{
	VSout vs;
	vs.pos = mul(pos, transform);
	vs.colour = col;
	return vs;
}