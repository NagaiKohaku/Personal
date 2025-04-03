#include "object2d.hlsli"

//マテリアル
struct Material
{
    float4 color;
    int enableLighting;
    float4x4 uvTransform;
};

//マテリアル
ConstantBuffer<Material> gMaterial : register(b0);

//テクスチャ
Texture2D<float4> gTexture : register(t0);

//サンプラー
SamplerState gSampler : register(t0);

//出力
struct PixelShaderOutPut
{
    float4 color : SV_TARGET0;
};

PixelShaderOutPut main(VertexShaderOutput input)
{
    //出力
    PixelShaderOutPut output;

    //UV情報
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);

    //テクスチャの色
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

    //アルファ値が0であれば
    if (textureColor.a == 0.0)
    {
        //処理をスキップ
        discard;
    }

    //マテリアル情報とテクスチャの色を合わせる
    output.color = gMaterial.color * textureColor;
 
    return output;
}