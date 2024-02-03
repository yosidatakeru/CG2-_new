struct VertexShaderOutput
{
    float32_t4 position : SV_POSITION;
	float32_t2 texcoord : TEXCOORD0;
	float32_t3 normal : NORMAL;
	float32_t3 worldPosition : POSITIONO;
};