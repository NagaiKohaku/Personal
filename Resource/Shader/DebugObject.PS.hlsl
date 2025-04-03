#include "DebugObject.hlsli"

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

PixelShaderOutPut main(VertexShaderOutput input)
{
    //出力データ
    PixelShaderOutPut output;

    //出力データにマテリアルの色を設定する
    output.color = gMaterial.color;

    return output;
}