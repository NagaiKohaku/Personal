#include "DebugObject.hlsli"

//����
struct VertexShaderInput
{
    //�s�N�Z�����W : RTV0
    float4 position : POSITION0;
};

//���W�s��
struct TransformationMatrix
{
    float4x4 WVP;
};

//���W�s�� : CBV0
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

VertexShaderOutput main(VertexShaderInput input)
{
    //�o�̓f�[�^
    VertexShaderOutput output;

    //�o�̓f�[�^�Ƀ��[���h���W��ݒ�
    output.position = mul(input.position, gTransformationMatrix.WVP);

    return output;
}