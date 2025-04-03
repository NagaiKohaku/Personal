#include "object3d.hlsli"

///=== 構造体 ===///

//マテリアル
struct Material
{
    float4 color;
    int enableLighting;
    float4x4 uvTransform;
    float shininess;
};

//平行光源ライト
struct DirectionalLight
{
    float4 color;
    float3 direction;
    float intensity;
};

//点光源ライト
struct PointLight
{
    float4 color;
    float3 position;
    float intensity;
    float radius;
    float decay;
};

//スポットライト
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

//カメラ
struct Camera
{
    float3 worldPosition;
};

//アウトプット
struct PixelShaderOutPut
{
    float4 color : SV_TARGET0;
};

///=== 変数 ===///

//マテリアル
ConstantBuffer<Material> gMaterial : register(b0);

//テクスチャ
Texture2D<float4> gTexture : register(t0);

//サンプラー
SamplerState gSampler : register(t0);

//カメラ
ConstantBuffer<Camera> gCamera : register(b1);

//平行光源ライト
ConstantBuffer<DirectionalLight> gDirectionLight : register(b2);

//点光源ライト
ConstantBuffer<PointLight> gPointLight : register(b3);

//スポットライト
ConstantBuffer<SpotLight> gSpotLight : register(b4);

///=== 関数 ===///

//内積
float Dot(float3 v1, float3 v2);

//平行光源ライトの計算
float3 DirectionalLightReflection(VertexShaderOutput input, float4 textureColor);

//点光源ライトの計算
float3 PointLightReflection(VertexShaderOutput input, float4 textureColor);

//スポットライトの計算
float3 SpotLightReflection(VertexShaderOutput input, float4 textureColor);

PixelShaderOutPut main(VertexShaderOutput input)
{
    //アウトプット
    PixelShaderOutPut output;
 
    //UV情報
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);

    //テクスチャの色
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

    //テクスチャのアルファ値が0であれば
    if (textureColor.a == 0.0)
    {
        //処理をスキップ
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

//平行光源ライトの計算
float3 DirectionalLightReflection(VertexShaderOutput input, float4 textureColor)
{

    ///=== 拡散反射の計算 ===///

    //コサイン値
    float cos = pow(Dot(input.normal,-gDirectionLight.direction) * 0.5f + 0.5f, 2.0f);

    //拡散反射
    float3 diffuse =
        gMaterial.color.rgb * textureColor.rgb * gDirectionLight.color.rgb * cos * gDirectionLight.intensity;

    ///=== 鏡面反射の計算 ===///

    //カメラへの方向
    float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);

    //カメラへの反射光(ハーフベクトル)
    float3 halfVector = normalize(-gDirectionLight.direction + toEye);

    //反射強度
    float specularPow = pow(saturate(Dot(input.normal,halfVector)), gMaterial.shininess);

    //鏡面反射
    float3 specular =
        gDirectionLight.color.rgb * gDirectionLight.intensity * specularPow * float3(1.0f, 1.0f, 1.0f);
 
    return diffuse + specular;
}

//点光源ライトの計算
float3 PointLightReflection(VertexShaderOutput input, float4 textureColor)
{

    ///=== 拡散反射の計算 ===///
        
    //点光源ライトの入射光
    float3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);

    //コサイン値
    float cos = pow(Dot(input.normal,-pointLightDirection) * 0.5f + 0.5f, 2.0f);

    //頂点座標とライトの距離
    float distance = length(gPointLight.position - input.worldPosition);

    //減衰係数
    float factor = pow(saturate(-distance / gPointLight.radius + 1.0), gPointLight.decay);

    //拡散反射
    float3 diffuse =
        gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * cos * gPointLight.intensity * factor;
 
    ///=== 鏡面反射の計算 ===///

    //カメラへの方向
    float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);

    //カメラへの反射光(ハーフベクトル)
    float3 halfVector = normalize(-pointLightDirection + toEye);

    //反射強度
    float specularPow = pow(saturate(Dot(input.normal,halfVector)), gMaterial.shininess);

    //鏡面反射
    float3 specular =
        gPointLight.color.rgb * gPointLight.intensity * factor * specularPow * float3(1.0f, 1.0f, 1.0f);

    return diffuse + specular;
}

//スポットライトの計算
float3 SpotLightReflection(VertexShaderOutput input, float4 textureColor)
{

    //スポットライトの入射光
    float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight.position);

    //距離
    float distance = length(gSpotLight.position - input.worldPosition);

    //距離減衰
    float attenuationFactor = pow(saturate(-distance / gSpotLight.distance + 1.0), gSpotLight.decay);

    ///=== 拡散反射の計算 ===///

    //方向
    float cosAngle = dot(spotLightDirectionOnSurface, gSpotLight.direction);

    //拡散率
    float falloffFactor = saturate((cosAngle - gSpotLight.cosAngle) / (1.0f + gSpotLight.cosFalloffStart - gSpotLight.cosAngle));

    //コサイン値
    float cos = pow(Dot(input.normal,-gSpotLight.direction) * 0.5f + 0.5f, 2.0f);

    //拡散反射
    float3 diffuse =
        gMaterial.color.rgb * textureColor.rgb * gSpotLight.color.rgb * cos * gSpotLight.intensity * attenuationFactor * falloffFactor;
 
    ///=== 鏡面反射の計算 ===///

    //カメラへの方向
    float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);

    //カメラへの反射光(ハーフベクトル)
    float3 halfVector = normalize(-gSpotLight.direction + toEye);

    //反射強度
    float specularPow = pow(saturate(Dot(input.normal,halfVector)), gMaterial.shininess);

    //鏡面反射
    float3 specular =
        gSpotLight.color.rgb * gSpotLight.intensity * attenuationFactor * falloffFactor * specularPow * float3(1.0f, 1.0f, 1.0f);

    return diffuse + specular;
}

//内積
float Dot(float3 v1, float3 v2)
{
    return dot(normalize(v1), v2);
}