#include "Vignette.hlsli"

//出力データ
struct PixelShaderOutPut
{
    //色 : RTV0
    float4 color : SV_TARGET0;
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutPut main(VertexShaderOutput input)
{
    //出力データ
    PixelShaderOutPut output;

    //出力データにマテリアルの色を設定する
    output.color = gTexture.Sample(gSampler, input.texcoord);

    float2 correct = input.texcoord * (1.0f - input.texcoord.yx);

    float vignette = correct.x * correct.y * 16.0f;
    
    vignette = saturate(pow(vignette, 0.8f));

    output.color.rgb *= vignette;

    return output;
}