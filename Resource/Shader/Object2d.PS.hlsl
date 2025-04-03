#include "object2d.hlsli"

//�}�e���A��
struct Material
{
    float4 color;
    int enableLighting;
    float4x4 uvTransform;
};

//�}�e���A��
ConstantBuffer<Material> gMaterial : register(b0);

//�e�N�X�`��
Texture2D<float4> gTexture : register(t0);

//�T���v���[
SamplerState gSampler : register(t0);

//�o��
struct PixelShaderOutPut
{
    float4 color : SV_TARGET0;
};

PixelShaderOutPut main(VertexShaderOutput input)
{
    //�o��
    PixelShaderOutPut output;

    //UV���
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);

    //�e�N�X�`���̐F
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

    //�A���t�@�l��0�ł����
    if (textureColor.a == 0.0)
    {
        //�������X�L�b�v
        discard;
    }

    //�}�e���A�����ƃe�N�X�`���̐F�����킹��
    output.color = gMaterial.color * textureColor;
 
    return output;
}