#include "CopyImage.hlsli"

//�o�̓f�[�^
struct PixelShaderOutPut
{
    //�F : RTV0
    float4 color : SV_TARGET0;
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

static const float2 kIndex9x9[9][9] =
{
    { { -4.0f, -4.0f }, { -3.0f, -4.0f }, { -2.0f, -4.0f }, { -1.0f, -4.0f }, { 0.0f, -4.0f }, { 1.0f, -4.0f }, { 2.0f, -4.0f }, { 3.0f, -4.0f }, { 4.0f, -4.0f } },
    { { -4.0f, -3.0f }, { -3.0f, -3.0f }, { -2.0f, -3.0f }, { -1.0f, -3.0f }, { 0.0f, -3.0f }, { 1.0f, -3.0f }, { 2.0f, -3.0f }, { 3.0f, -3.0f }, { 4.0f, -3.0f } },
    { { -4.0f, -2.0f }, { -3.0f, -2.0f }, { -2.0f, -2.0f }, { -1.0f, -2.0f }, { 0.0f, -2.0f }, { 1.0f, -2.0f }, { 2.0f, -2.0f }, { 3.0f, -2.0f }, { 4.0f, -2.0f } },
    { { -4.0f, -1.0f }, { -3.0f, -1.0f }, { -2.0f, -1.0f }, { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f }, { 2.0f, -1.0f }, { 3.0f, -1.0f }, { 4.0f, -1.0f } },
    { { -4.0f, 0.0f },  { -3.0f, 0.0f },  { -2.0f, 0.0f },  { -1.0f, 0.0f },  { 0.0f, 0.0f },  { 1.0f, 0.0f },  { 2.0f, 0.0f },  { 3.0f, 0.0f },  { 4.0f, 0.0f } },
    { { -4.0f, 1.0f },  { -3.0f, 1.0f },  { -2.0f, 1.0f },  { -1.0f, 1.0f },  { 0.0f, 1.0f },  { 1.0f, 1.0f },  { 2.0f, 1.0f },  { 3.0f, 1.0f },  { 4.0f, 1.0f } },
    { { -4.0f, 2.0f },  { -3.0f, 2.0f },  { -2.0f, 2.0f },  { -1.0f, 2.0f },  { 0.0f, 2.0f },  { 1.0f, 2.0f },  { 2.0f, 2.0f },  { 3.0f, 2.0f },  { 4.0f, 2.0f } },
    { { -4.0f, 3.0f },  { -3.0f, 3.0f },  { -2.0f, 3.0f },  { -1.0f, 3.0f },  { 0.0f, 3.0f },  { 1.0f, 3.0f },  { 2.0f, 3.0f },  { 3.0f, 3.0f },  { 4.0f, 3.0f } },
    { { -4.0f, 4.0f },  { -3.0f, 4.0f },  { -2.0f, 4.0f },  { -1.0f, 4.0f },  { 0.0f, 4.0f },  { 1.0f, 4.0f },  { 2.0f, 4.0f },  { 3.0f, 4.0f },  { 4.0f, 4.0f } }
};

static const float kKernel9x9[9][9] =
{
    { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f }
};

static const uint kKernelSize = 7;

static const uint kKernelMax = 9;

PixelShaderOutPut main(VertexShaderOutput input)
{
    uint width, height;
    gTexture.GetDimensions(width, height);
    float2 uvStepSize = float2(rcp(float(width)), rcp(float(height)));

    //�o�̓f�[�^
    PixelShaderOutPut output;

    output.color.rgb = float3(0.0f, 0.0f, 0.0f);

    output.color.a = 1.0f;

    uint kernelSize = (kKernelMax / 2.0f) - (kKernelSize / 2.0f);

    for (int x = 0; x < kKernelSize; ++x)
    {
        for (int y = 0; y < kKernelSize; ++y)
        {
            //���݂�texcoord���Z�o
            float2 texcoord = input.texcoord + kIndex9x9[kernelSize + x][kernelSize + y] * uvStepSize;
            
            //�F��1/9���|���ĉ��Z
            float3 fetColor = gTexture.Sample(gSampler, texcoord).rgb;
            output.color.rgb += saturate(fetColor * kKernel9x9[kernelSize + x][kernelSize + y] / pow(float(kKernelSize), 2.0f));
        }
    }

    return output;
}

