//頂点シェーダーの出力データ
struct VertexShaderOutput
{
    //座標データ
    float4 position : SV_POSITION;
    //テクスチャ座標
    float2 texcoord : TEXCOORD0;
};