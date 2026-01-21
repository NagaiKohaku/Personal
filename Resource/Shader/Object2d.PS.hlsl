#include "object2d.hlsli"

//マテリアル
struct Material
{
    float4 color;
    int enableLighting;
    int enableEdit;
    float4x4 uvTransform;
    float ratio;
    float brightness;
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

float4 FXAA_PS(VertexShaderOutput input);

PixelShaderOutPut main(VertexShaderOutput input)
{
    //出力
    PixelShaderOutPut output;

    //UV情報
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);

    //テクスチャの色
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

    if (gMaterial.enableEdit == 1)
    {

        float borderPixel = 2.0f;

        float2 uvPerPixel;
        uvPerPixel.x = abs(ddx(input.texcoord.x));
        uvPerPixel.y = abs(ddy(input.texcoord.y));

        float2 borderUV = uvPerPixel * borderPixel;

        bool isBorder =
            input.texcoord.x < borderUV.x ||
            input.texcoord.x > 1.0f - borderUV.x ||
            input.texcoord.y < borderUV.y ||
            input.texcoord.y > 1.0f - borderUV.y;

        if (isBorder)
        {
            output.color = float4(1, 1, 1, 1);
            return output;
        }
    }

    //アルファ値が0であれば
    if (textureColor.a == 0.0)
    {
        //処理をスキップ
        discard;
    }

    //マテリアル情報とテクスチャの色を合わせる
    output.color = gMaterial.color * FXAA_PS(input);

    float isUnderRatio = step(input.texcoord.x, gMaterial.ratio);

    output.color.rgb *= lerp(gMaterial.brightness, 1.0f, isUnderRatio);

    return output;
}

float4 FXAA_PS(VertexShaderOutput input)
{

    float2 inverseScreenSize = float2(1.0f / 1280.0f, 1.0f / 720.0f);

    float2 uv = input.texcoord;

    // 周囲4ピクセル＋中心をサンプル
    float4 center = gTexture.Sample(gSampler, uv);
    float4 nw = gTexture.Sample(gSampler, uv + float2(-1, -1) * inverseScreenSize);
    float4 ne = gTexture.Sample(gSampler, uv + float2(1, -1) * inverseScreenSize);
    float4 sw = gTexture.Sample(gSampler, uv + float2(-1, 1) * inverseScreenSize);
    float4 se = gTexture.Sample(gSampler, uv + float2(1, 1) * inverseScreenSize);

    // Premultiplied Alphaでブレンド
    float3 rgb = (center.rgb * center.a + nw.rgb * nw.a + ne.rgb * ne.a + sw.rgb * sw.a + se.rgb * se.a) /
                 (center.a + nw.a + ne.a + sw.a + se.a + 1e-6);

    float alpha = center.a * gMaterial.color.a;

    return float4(rgb, alpha);
}