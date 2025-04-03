#include "DebugObject.hlsli"

//入力
struct VertexShaderInput
{
    //ピクセル座標 : RTV0
    float4 position : POSITION0;
};

//座標行列
struct TransformationMatrix
{
    float4x4 WVP;
};

//座標行列 : CBV0
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

VertexShaderOutput main(VertexShaderInput input)
{
    //出力データ
    VertexShaderOutput output;

    //出力データにワールド座標を設定
    output.position = mul(input.position, gTransformationMatrix.WVP);

    return output;
}