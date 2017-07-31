#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 1, binding = 0) uniform modelUniform{
mat4 m;
} model;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
	mat4 mdl = model.m;
	//Multiply by -1 because glm was made for opengl
	mdl[1][1] *= -1;
    gl_Position = /*model.m*/ mdl * vec4(inPosition.xy, 0.0,  1.0);
    fragTexCoord = inTexCoord;
}