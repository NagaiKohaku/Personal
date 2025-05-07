#include "GrayScale.hlsli"

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

    //マテリアルの色を取得
    output.color = gTexture.Sample(gSampler, input.texcoord);

    //マテリアルのRGB値に対して、緑高め青低めの加重平均を計算
    float value = dot(output.color.rgb, float3(0.2125f, 0.7154f, 0.0721f));

    //出力データの色をグレースケールにする
    output.color.rgb = float3(value, value, value);

    return output;
}