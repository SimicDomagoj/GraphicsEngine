#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UniformBufferObject {
	mat4 pv;
} ubo;

layout(set = 1, binding = 0) uniform modelUniform{
mat4 m;
} model;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUv;

layout(location = 0) out vec2 outUv;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
	outUv = inUv;
	gl_Position = ubo.pv * model.m * vec4(inPosition, 1.0);
}