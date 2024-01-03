/*
struct VSOut {
	float4 pos : SV_Position;
};
//
cbuffer Cbuf {
	matrix transform;
};
*/

float4 main(float2 pos : Position) : SV_Position {
	return float4(pos.x, pos.y, 0.0f, 1.0f);
}