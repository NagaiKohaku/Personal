#include "object3d.hlsli"

///=== �\���� ===///

//�}�e���A��
struct Material
{
    float4 color;
    int enableLighting;
    float4x4 uvTransform;
    float shininess;
};

//���s�������C�g
struct DirectionalLight
{
    float4 color;
    float3 direction;
    float intensity;
};

//�_�������C�g
struct PointLight
{
    float4 color;
    float3 position;
    float intensity;
    float radius;
    float decay;
};

//�X�|�b�g���C�g
struct SpotLight
{
    float4 color;
    float3 position;
    float intensity;
    float3 direction;
    float distance;
    float decay;
    float cosAngle;
    float cosFalloffStart;
};

//�J����
struct Camera
{
    float3 worldPosition;
};

//�A�E�g�v�b�g
struct PixelShaderOutPut
{
    float4 color : SV_TARGET0;
};

///=== �ϐ� ===///

//�}�e���A��
ConstantBuffer<Material> gMaterial : register(b0);

//�e�N�X�`��
Texture2D<float4> gTexture : register(t0);

//�T���v���[
SamplerState gSampler : register(t0);

//�J����
ConstantBuffer<Camera> gCamera : register(b1);

//���s�������C�g
ConstantBuffer<DirectionalLight> gDirectionLight : register(b2);

//�_�������C�g
ConstantBuffer<PointLight> gPointLight : register(b3);

//�X�|�b�g���C�g
ConstantBuffer<SpotLight> gSpotLight : register(b4);

///=== �֐� ===///

//����
float Dot(float3 v1, float3 v2);

//���s�������C�g�̌v�Z
float3 DirectionalLightReflection(VertexShaderOutput input, float4 textureColor);

//�_�������C�g�̌v�Z
float3 PointLightReflection(VertexShaderOutput input, float4 textureColor);

//�X�|�b�g���C�g�̌v�Z
float3 SpotLightReflection(VertexShaderOutput input, float4 textureColor);

PixelShaderOutPut main(VertexShaderOutput input)
{
    //�A�E�g�v�b�g
    PixelShaderOutPut output;
 
    //UV���
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);

    //�e�N�X�`���̐F
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

    //�e�N�X�`���̃A���t�@�l��0�ł����
    if (textureColor.a == 0.0)
    {
        //�������X�L�b�v
        discard;
    }
    
    if (textureColor.a <= 0.5)
    {
        discard;
    }
    
    if (output.color.a == 0.0)
    {
        discard;
    }

    if (gMaterial.enableLighting != 0)
    {
        output.color.rgb =
        DirectionalLightReflection(input, textureColor) + 
        PointLightReflection(input, textureColor) + 
        SpotLightReflection(input, textureColor);

        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
 
    return output;
}

//���s�������C�g�̌v�Z
float3 DirectionalLightReflection(VertexShaderOutput input, float4 textureColor)
{

    ///=== �g�U���˂̌v�Z ===///

    //�R�T�C���l
    float cos = pow(Dot(input.normal,-gDirectionLight.direction) * 0.5f + 0.5f, 2.0f);

    //�g�U����
    float3 diffuse =
        gMaterial.color.rgb * textureColor.rgb * gDirectionLight.color.rgb * cos * gDirectionLight.intensity;

    ///=== ���ʔ��˂̌v�Z ===///

    //�J�����ւ̕���
    float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);

    //�J�����ւ̔��ˌ�(�n�[�t�x�N�g��)
    float3 halfVector = normalize(-gDirectionLight.direction + toEye);

    //���ˋ��x
    float specularPow = pow(saturate(Dot(input.normal,halfVector)), gMaterial.shininess);

    //���ʔ���
    float3 specular =
        gDirectionLight.color.rgb * gDirectionLight.intensity * specularPow * float3(1.0f, 1.0f, 1.0f);
 
    return diffuse + specular;
}

//�_�������C�g�̌v�Z
float3 PointLightReflection(VertexShaderOutput input, float4 textureColor)
{

    ///=== �g�U���˂̌v�Z ===///
        
    //�_�������C�g�̓��ˌ�
    float3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);

    //�R�T�C���l
    float cos = pow(Dot(input.normal,-pointLightDirection) * 0.5f + 0.5f, 2.0f);

    //���_���W�ƃ��C�g�̋���
    float distance = length(gPointLight.position - input.worldPosition);

    //�����W��
    float factor = pow(saturate(-distance / gPointLight.radius + 1.0), gPointLight.decay);

    //�g�U����
    float3 diffuse =
        gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * cos * gPointLight.intensity * factor;
 
    ///=== ���ʔ��˂̌v�Z ===///

    //�J�����ւ̕���
    float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);

    //�J�����ւ̔��ˌ�(�n�[�t�x�N�g��)
    float3 halfVector = normalize(-pointLightDirection + toEye);

    //���ˋ��x
    float specularPow = pow(saturate(Dot(input.normal,halfVector)), gMaterial.shininess);

    //���ʔ���
    float3 specular =
        gPointLight.color.rgb * gPointLight.intensity * factor * specularPow * float3(1.0f, 1.0f, 1.0f);

    return diffuse + specular;
}

//�X�|�b�g���C�g�̌v�Z
float3 SpotLightReflection(VertexShaderOutput input, float4 textureColor)
{

    //�X�|�b�g���C�g�̓��ˌ�
    float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight.position);

    //����
    float distance = length(gSpotLight.position - input.worldPosition);

    //��������
    float attenuationFactor = pow(saturate(-distance / gSpotLight.distance + 1.0), gSpotLight.decay);

    ///=== �g�U���˂̌v�Z ===///

    //����
    float cosAngle = dot(spotLightDirectionOnSurface, gSpotLight.direction);

    //�g�U��
    float falloffFactor = saturate((cosAngle - gSpotLight.cosAngle) / (1.0f + gSpotLight.cosFalloffStart - gSpotLight.cosAngle));

    //�R�T�C���l
    float cos = pow(Dot(input.normal,-gSpotLight.direction) * 0.5f + 0.5f, 2.0f);

    //�g�U����
    float3 diffuse =
        gMaterial.color.rgb * textureColor.rgb * gSpotLight.color.rgb * cos * gSpotLight.intensity * attenuationFactor * falloffFactor;
 
    ///=== ���ʔ��˂̌v�Z ===///

    //�J�����ւ̕���
    float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);

    //�J�����ւ̔��ˌ�(�n�[�t�x�N�g��)
    float3 halfVector = normalize(-gSpotLight.direction + toEye);

    //���ˋ��x
    float specularPow = pow(saturate(Dot(input.normal,halfVector)), gMaterial.shininess);

    //���ʔ���
    float3 specular =
        gSpotLight.color.rgb * gSpotLight.intensity * attenuationFactor * falloffFactor * specularPow * float3(1.0f, 1.0f, 1.0f);

    return diffuse + specular;
}

//����
float Dot(float3 v1, float3 v2)
{
    return dot(normalize(v1), v2);
}