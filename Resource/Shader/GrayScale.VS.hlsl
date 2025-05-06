#include "GrayScale.hlsli"

static const uint kNumVertex = 3;

static const float4 kPositions[kNumVertex] =
{
    { -1.0f, 1.0f, 0.0f, 1.0f }, //����
    { 3.0f, 1.0f, 0.0f, 1.0f },  //�E��
    { -1.0f, -3.0f, 0.0f, 1.0f } //����
};

static const float2 kTexCoords[kNumVertex] =
{
    { 0.0f, 0.0f }, //����
    { 2.0f, 0.0f }, //�E��
    { 0.0f, 2.0f }  //����
};

VertexShaderOutput main(uint vertexId : SV_VertexID)
{
    //�o�̓f�[�^
    VertexShaderOutput output;

    //�o�̓f�[�^�Ƀ��[���h���W��ݒ�
    output.position = kPositions[vertexId];

    //�o�̓f�[�^�Ƀe�N�X�`�����W��ݒ�
    output.texcoord = kTexCoords[vertexId];
    
    return output;
}