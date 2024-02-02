struct TransformationMatrix
{
	float32_t4x4 WVP;
	float32_t4x4 World;
};

//CBuffer
StructuredBuffer<TransformationMatrix>gTransformationMatrices:register(t0);


struct VertexShaderOutput 
{
	float32_t4 position : SV_POSITION;
};



struct VertexShaderInput 
{
	float32_t4 position : POSITION0;
};




VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceID)
{
	VertexShaderOutput output;
	output.position = mul(input.position, gTransformationMatrices[instanceId].WVP);
	//output.texcoord = input.texcoord;
	//output.normal = normalize(mul(input.normal, (float32_t3x3)gTransformationMatrices[instanceId].World));
	return output;
}
