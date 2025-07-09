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

    //マテリアルの色を取得
    output.color = gTexture.Sample(gSampler, input.texcoord);

    //中心に近づくほど、明るくなるように計算
    float2 correct = input.texcoord * (1.0f - input.texcoord.yx);

    //中心の最大値が1.0fになるように調整
    float vignette = correct.x * correct.y * 16.0f;

    //0.8乗して効果を薄める
    vignette = saturate(pow(vignette, 0.8f));

    //RBG値にVignetteを掛ける
    output.color.rgb *= vignette;

    return output;
}