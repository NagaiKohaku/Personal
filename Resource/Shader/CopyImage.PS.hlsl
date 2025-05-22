#include "CopyImage.hlsli"

//�o�̓f�[�^
struct PixelShaderOutPut
{
    //�F : RTV0
    float4 color : SV_TARGET0;
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutPut main(VertexShaderOutput input)
{
    //�o�̓f�[�^
    PixelShaderOutPut output;

    //�o�̓f�[�^�Ƀ}�e���A���̐F��ݒ肷��
    output.color = gTexture.Sample(gSampler, input.texcoord);

    return output;
}