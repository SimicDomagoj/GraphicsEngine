#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 1, binding = 1) uniform sampler2D texSampler;
layout(set = 1, binding = 2) uniform sampler2D normalMap;
layout(set = 1, binding = 3) uniform sampler2D depthMap;

layout(location = 0) in vec2 uv;
layout(location = 1) in vec3 viewDirection_tangentSpace;
layout(location = 2) in vec3 lightDirection_tangentSpace;
layout(location = 3) in vec3 halfVector_tangentSpace;

layout(location = 0) out vec4 outColor;

float heightScale = 0.1;
const float minLayers = 8;
const float maxLayers = 32;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
	const float minLayers = 10;
    const float maxLayers = 20;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * heightScale; 
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  currentTexCoords = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }
    
    // -- parallax occlusion mapping interpolation from here on
    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;

    //float height =  texture(depthMap, texCoords).r;    
    //vec2 p = viewDir.xy /* viewDir.z*/ * (height * heightScale);
    //return texCoords - p; 
} 

void main() {
	vec3 LightColor = vec3(1.0,1.0,1.0);
	float LightPower = 40.0;

	vec3 MaterialDiffuseColor = texture( texSampler, uv ).rgb;
	vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = texture( texSampler, uv ).rgb * 0.3;

	vec3 viewDir = normalize(viewDirection_tangentSpace);
	vec2 texCoords = ParallaxMapping(uv,  viewDir);
	if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
	{
    	discard;
    }

	vec3 texNormal_tangentspace = texture( normalMap, texCoords ).rgb*2.0 - 1.0;

	//TODO put distance
	vec3 N = normalize(texNormal_tangentspace);
	vec3 L = normalize(lightDirection_tangentSpace);
	float cosNL = clamp(dot(N, L), 0.0, 1.0);

	vec3 H = normalize(halfVector_tangentSpace);
	float cosHN = clamp(dot(H, N), 0.0, 1.0);

	vec3 color = 
		MaterialAmbientColor +
		MaterialDiffuseColor * LightColor * cosNL +
		MaterialSpecularColor * LightColor * pow(cosHN,16);

	outColor = vec4(color, 1.0);
}