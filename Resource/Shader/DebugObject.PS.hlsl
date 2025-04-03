#include "DebugObject.hlsli"

//�o�̓f�[�^
struct PixelShaderOutPut
{
    //�F : RTV0
    float4 color : SV_TARGET0;
};

//�}�e���A��
struct Material
{
    //�F
    float4 color;
};

//�}�e���A�� : CBV0
ConstantBuffer<Material> gMaterial : register(b0);

PixelShaderOutPut main(VertexShaderOutput input)
{
    //�o�̓f�[�^
    PixelShaderOutPut output;

    //�o�̓f�[�^�Ƀ}�e���A���̐F��ݒ肷��
    output.color = gMaterial.color;

    return output;
}