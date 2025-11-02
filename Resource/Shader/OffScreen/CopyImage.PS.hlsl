#include "CopyImage.hlsli"

//出力データ
struct PixelShaderOutPut
{
    //色 : RTV0
    float4 color : SV_TARGET0;
};

struct Material
{
    float4x4 projectionInverse;
    float colorReverseRatio;
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

ConstantBuffer<Material> gMaterial : register(b0);

PixelShaderOutPut main(VertexShaderOutput input)
{
    //出力データ
    PixelShaderOutPut output;

    //出力データにマテリアルの色を設定する
    float4 color = gTexture.Sample(gSampler, input.texcoord);

    // 反転色を計算（アルファは維持）
    float3 inverted = 1.0f - color.rgb * 2.0f;

    float3 result = lerp(color.rgb, inverted, gMaterial.colorReverseRatio);

    output.color = float4(result, color.a);

    return output;
}
