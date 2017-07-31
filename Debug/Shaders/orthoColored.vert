#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform orthoUniform {
    mat4 matrix;
} ortho;

layout(set = 1, binding = 0) uniform modelUniform{
mat4 m;
} model;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    gl_Position = ortho.matrix * model.m * vec4(inPosition, 0.0,  1.0);
    fragColor = inColor;
}