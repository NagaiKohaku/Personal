#include "SkyBox.hlsli"

//出力データ
struct PixelShaderOutPut
{
    //色 : RTV0
    float4 color : SV_TARGET0;
};

//マテリアル
struct Material
{
    //色
    float4 color;
};

//マテリアル : CBV0
ConstantBuffer<Material> gMaterial : register(b0);

//テクスチャ
TextureCube<float4> gTexture : register(t0);

//サンプラー
SamplerState gSampler : register(t0);

PixelShaderOutPut main(VertexShaderOutput input)
{
    //出力データ
    PixelShaderOutPut output;

    //テクスチャから色を取得する
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);

    //出力データにマテリアルの色を設定する
    output.color = textureColor * gMaterial.color;

    return output;
}