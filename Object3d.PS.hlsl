struct Material
{
	float32_t4 color;
};
ConstantBuffer<Material>gMaterial :register(b0);


struct PixelShaderOutput 
{
	float32_t4 color : SV_TARGET0;
};


PixelShaderOutput main() 
{
	PixelShaderOutput output;
	output.color =   gMaterial.color;//float32_t4(30.0, 0.0, 0.0, 0.0);

	return output;
}