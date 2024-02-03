#include "Object3d.hlsli"
struct Material
{
	float32_t4 color;
	int32_t enableLighting;
};

struct DirectionalLigha
{
	float32_t4 color;//ライトの色
	float32_t3 direction;//!<ライトの向き
	float intensity; //!<輝度
};

ConstantBuffer<Material>gMaterial :register(b0);
ConstantBuffer<DirectionalLigha>gDirectionalLigha :register(b1);


struct PixelShaderOutput 
{
	float32_t4 color : SV_TARGET0;
};


PixelShaderOutput main(VertexShaderOutput input)
{
	PixelShaderOutput output;

	

	if (gMaterial.enableLighting != 0)
	{
		float cos = saturate(dot(normalize(input.normal), -gDirectionalLigha.direction));
		output.color = gMaterial.color * gDirectionalLigha.color * cos * gDirectionalLigha.intensity;

	}else
	{
		output.color = gMaterial.color;
	}
	//output.color = gMaterial.color;
	return output;
}