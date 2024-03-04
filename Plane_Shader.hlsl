//define input as struct
//ORDER IS SUPER IMPORTANT
struct VSout {
	float3 colour : Colour;
	//marked as sv pos so shader knows this is the pos output
	float4 pos : SV_Position;
};
//collect 2 bound transforms
//ORDER IS SUPER IMPORTANT AGIAN
cbuffer transform {
	float4x4 t_view;
};
cbuffer transform {
	float4x4 t_plane;
};
//take input
VSout main(float4 pos : Position, float3 col : Colour) {
	//transfrom plane points using matrix bound in scene2d
	float4 temp = mul(pos, t_plane);
	//create local struct so i can avoid the col info
	VSout vs;
	//transform to cannonical view volume
	vs.pos = mul(temp, t_view);
	//get input colour
	vs.colour = col;
	//return both the colour and the positions
	return vs;
}