#include "Object3d.hlsli"
struct Material
{
	float32_t4 color;
	int32_t enableLighting;
	float32_t shininess;
};

struct DirectionalLigha
{
	float32_t4 color;//ライトの色
	float32_t3 direction;//!<ライトの向き
	float intensity; //!<輝度
};
struct Camera
{
	float32_t worldPosition;
};
ConstantBuffer<Material>gMaterial :register(b0);
ConstantBuffer<DirectionalLigha>gDirectionalLigha :register(b1);
ConstantBuffer<Camera>gCamera :register(b2);




struct PixelShaderOutput 
{
	float32_t4 color : SV_TARGET0;
};



PixelShaderOutput main(VertexShaderOutput input)
{
	PixelShaderOutput output;

	

	if (gMaterial.enableLighting != 0)
	{
		float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
		float32_t3 reflectLight = reflect(gDirectionalLigha.direction, normalize(input.normal));
		float RdotE = dot(reflectLight, toEye);
		float specularPow = pow(saturate(RdotE), gMaterial.shininess);

		//float cos = saturate(dot(normalize(input.normal), -gDirectionalLigha.direction));
		float NdotL = dot(normalize(input.normal), -gDirectionalLigha.direction);
		float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
		//output.color = gMaterial.color * gDirectionalLigha.color * cos * gDirectionalLigha.intensity;
		float32_t3 diffuse =
			gMaterial.color.rgb * gDirectionalLigha.color.rgb * cos * gDirectionalLigha.intensity;
		
		float32_t3 specular =
		gDirectionalLigha.color.rgb * gDirectionalLigha.intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
		output.color.rgb = diffuse + specular;
		output.color.a = gMaterial.color.a;


	}else
	{
		output.color = gMaterial.color;
	}
	//output.color = gMaterial.color;
	return output;
}