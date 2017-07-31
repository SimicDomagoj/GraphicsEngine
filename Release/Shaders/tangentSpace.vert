#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UniformBufferObject {
	mat4 pv;
} ubo;

layout(set = 0, binding = 1) uniform UniformLightObject {
	vec3 position;
} light;

layout(set = 0, binding = 2) uniform View {
	vec3 position;
} view;

layout(set = 1, binding = 0) uniform modelUniform{
	mat4 m;
} model;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUv;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBitangent;

layout(location = 0) out vec2 outUv;
layout(location = 1) out vec3 outViewDirection_tangentSpace;
layout(location = 2) out vec3 outLightVec_tangentSpace;
layout(location = 3) out vec3 outHalfVec_tangentSpace;

void main(){
	vec4 vertex_worldSpace = model.m * vec4(inPosition, 1.0);
	gl_Position = ubo.pv * vertex_worldSpace;
	outUv = inUv;

	vec3 lightPos_worldSpace = light.position;

	vec3 lightDir_worldSpace = lightPos_worldSpace - vertex_worldSpace.xyz;
	vec3 halfVec_worldSpace = normalize(lightDir_worldSpace + vertex_worldSpace.xyz);
	vec3 viewDirection_worldSpace = view.position - vertex_worldSpace.xyz;

	mat3 m = mat3(model.m);

	vec3 tangent_worldSpace = normalize(m * inTangent);
	vec3 bitangent_worldSpace = normalize(m * inBitangent);
	vec3 normal_worldSpace = normalize(m * inNormal);

	//mat3 TBN = transpose(mat3(tangent_worldSpace, bitangent_worldSpace, normal_worldSpace));

	outLightVec_tangentSpace.x = dot(tangent_worldSpace, lightDir_worldSpace);
	outLightVec_tangentSpace.y = dot(bitangent_worldSpace, lightDir_worldSpace);
	outLightVec_tangentSpace.z = dot(normal_worldSpace, lightDir_worldSpace);

	outHalfVec_tangentSpace.x = dot(tangent_worldSpace, halfVec_worldSpace);
	outHalfVec_tangentSpace.y = dot(bitangent_worldSpace, halfVec_worldSpace);
	outHalfVec_tangentSpace.z = dot(normal_worldSpace, halfVec_worldSpace);

	outViewDirection_tangentSpace.x = dot(tangent_worldSpace, viewDirection_worldSpace);
	outViewDirection_tangentSpace.y = dot(bitangent_worldSpace, viewDirection_worldSpace);
	outViewDirection_tangentSpace.z = dot(normal_worldSpace, viewDirection_worldSpace);

	/*outLightVec_tangentSpace = TBN * lightDir_worldSpace;
	outHalfVec_tangentSpace =  TBN * halfVec_worldSpace;
	outViewDirection_tangentSpace = TBN * viewDirection_worldSpace;*/
}