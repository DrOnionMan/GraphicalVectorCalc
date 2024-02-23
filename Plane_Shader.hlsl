struct VSout {
	float3 colour : Colour;
	float4 pos : SV_Position;
};

cbuffer transform {
	float4x4 t_view;
};

cbuffer transform {
	float4x4 t_plane;
};

VSout main(float4 pos : Position, float3 col : Colour)
{
	float4 temp = mul(pos, t_plane);
	VSout vs;
	vs.pos = mul(temp, t_view);
	vs.colour = col;
	return vs;
}