#include "GrayScale.hlsli"

static const uint kNumVertex = 3;

static const float4 kPositions[kNumVertex] =
{
    { -1.0f, 1.0f, 0.0f, 1.0f }, //左上
    { 3.0f, 1.0f, 0.0f, 1.0f },  //右上
    { -1.0f, -3.0f, 0.0f, 1.0f } //左下
};

static const float2 kTexCoords[kNumVertex] =
{
    { 0.0f, 0.0f }, //左上
    { 2.0f, 0.0f }, //右上
    { 0.0f, 2.0f }  //左下
};

VertexShaderOutput main(uint vertexId : SV_VertexID)
{
    //出力データ
    VertexShaderOutput output;

    //出力データにワールド座標を設定
    output.position = kPositions[vertexId];

    //出力データにテクスチャ座標を設定
    output.texcoord = kTexCoords[vertexId];
    
    return output;
}