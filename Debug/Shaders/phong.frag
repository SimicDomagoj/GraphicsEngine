#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 1, binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 normal;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 viewVec;
layout(location = 3) in vec3 lightVec;

layout(location = 0) out vec4 outColor;

void main() {
	vec3 N = normalize(normal);
	vec3 L = normalize(lightVec);
	vec3 E = normalize(viewVec);
	vec3 R = normalize(-reflect(L, N));
	float cosNL = clamp(dot(N, L), 0.0, 1.0);
	float cosER = clamp(dot(E, R), 0.0, 1.0);

	vec3 color = texture(texSampler, uv).rgb;
	vec3 ambient = color * vec3(0.1);
	vec3 diffuse = cosNL * color;
	vec3 specular = pow(cosER, 16.0) * vec3(0.75);
	outColor = vec4(ambient + diffuse + specular, 1.0);
}