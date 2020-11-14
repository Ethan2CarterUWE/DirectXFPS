/*struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};



VOut main(float4 position : POSITION, float4 color : COLOR)
{
    VOut output;

    output.position = position;
    output.color = color;
    //output.color = 1.0f - color;    // set the output color to the inverted input color
    //output.color.g = 1.0f;    // maximize the red in each vertex
    //output.color.b = 1.0f;    // maximize the red in each vertex

    // output.color = 0.5f - color;    // set the output color to the inverted input color
    return output;
}*/

struct VSOut
{
    float3 color : Color;

    float4 pos : SV_Position;

};

VSOut main(float2 pos : Position, float3 color : Color)
{
    VSOut vso;
    vso.pos = float4(pos.x, pos.y, 0.0f, 1.0f);
    vso.color = color;
    return vso;
}