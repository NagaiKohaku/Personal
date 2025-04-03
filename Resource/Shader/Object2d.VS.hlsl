#include "object2d.hlsli"

//座標行列
struct TransformationMatrix
{
    float4x4 WVP;
    float4x4 World;
};

//座標行列
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

//入力
struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    //出力
    VertexShaderOutput output;

    //座標
    output.position = mul(input.position, gTransformationMatrix.WVP);

    //テクスチャ番号
    output.texcoord = input.texcoord;

    return output;
}