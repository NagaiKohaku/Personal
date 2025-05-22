#include "GrayScale.hlsli"

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

    //�}�e���A���̐F���擾
    output.color = gTexture.Sample(gSampler, input.texcoord);

    //�}�e���A����RGB�l�ɑ΂��āA�΍��ߐ�߂̉��d���ς��v�Z
    float value = dot(output.color.rgb, float3(0.2125f, 0.7154f, 0.0721f));

    //�o�̓f�[�^�̐F���O���[�X�P�[���ɂ���
    output.color.rgb = float3(value, value, value);

    return output;
}