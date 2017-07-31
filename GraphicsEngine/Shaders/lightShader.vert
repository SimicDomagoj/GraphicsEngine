#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UniformBufferObject {
	mat4 pv;
} ubo;

layout(set = 0, binding = 1) uniform UniformLightObject {
	vec3 position;
} light;

layout(set = 1, binding = 0) uniform modelUniform{
	mat4 m;
} model;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUv;

layout(location = 0) out vec3 outNormal;
layout(location = 1) out vec2 outUv;
layout(location = 2) out vec3 outViewVec;
layout(location = 3) out vec3 outLightVec;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
	outUv = inUv;
	vec4 position_worldSpace = model.m * vec4(inPosition, 1.0);
	gl_Position = ubo.pv * position_worldSpace;
	outNormal = mat3(model.m) * inNormal;
	outLightVec = light.position - position_worldSpace.xyz;
	outViewVec = - position_worldSpace.xyz;
}