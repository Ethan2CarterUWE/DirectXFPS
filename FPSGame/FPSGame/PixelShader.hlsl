/*float4 main(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
//float4 main(float4 position : SV_POSITION) : SV_TARGET

{
    return color;
//return float4(0.5f, 0.3f, 0.0f, 1.0f);
}*/

float4 main(float3 color : Color) : SV_Target
{
	return float4(color,1.0f);
}