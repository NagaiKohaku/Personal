#include "Vignette.hlsli"

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

    //���S�ɋ߂Â��قǁA���邭�Ȃ�悤�Ɍv�Z
    float2 correct = input.texcoord * (1.0f - input.texcoord.yx);

    //���S�̍ő�l��1.0f�ɂȂ�悤�ɒ���
    float vignette = correct.x * correct.y * 16.0f;

    //0.8�悵�Č��ʂ𔖂߂�
    vignette = saturate(pow(vignette, 0.8f));

    //RBG�l��Vignette���|����
    output.color.rgb *= vignette;

    return output;
}