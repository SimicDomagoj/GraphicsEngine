#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 1, binding = 1) uniform sampler2D texSampler;
layout(set = 1, binding = 2) uniform sampler2D normalMap;

layout(location = 0) in vec2 uv;
layout(location = 1) in vec3 viewDirection_tangentSpace;
layout(location = 2) in vec3 lightDirection_tangentSpace;
layout(location = 3) in vec3 halfVector_tangentSpace;

layout(location = 0) out vec4 outColor;

void main() {
	vec3 LightColor = vec3(1.0,1.0,1.0);
	float LightPower = 40.0;

	vec3 MaterialDiffuseColor = texture( texSampler, uv ).rgb;
	vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = texture( texSampler, uv ).rgb * 0.3;

	vec3 texNormal_tangentspace = normalize(texture( normalMap, uv ).rgb*2.0 - 1.0);

	//TODO put distance
	vec3 N = texNormal_tangentspace;
	vec3 L = normalize(lightDirection_tangentSpace);
	float cosNL = clamp(dot(N, L), 0.0, 1.0);

	vec3 H = normalize(halfVector_tangentSpace);
	float cosHN = clamp(dot(H, N), 0.0, 1.0);

	vec3 color = 
		MaterialAmbientColor +
		MaterialDiffuseColor * LightColor * cosNL +
		MaterialSpecularColor * LightColor * pow(cosHN,5);

	outColor = vec4(color, 1.0);
}