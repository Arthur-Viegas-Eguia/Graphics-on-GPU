


#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform SceneUniforms {
    mat4 camera;
    vec4 uLight;
    vec4 cLight;
    vec4 pLight;
    vec4 cLight2;
    vec4 cAmbient;
    vec4 pCamera;
} scene;
layout(binding = 1) uniform BodyUniforms {
    mat4 modeling;
    uvec4 texIndices;
    vec4 cSpecular;
} body;
layout(binding = 2) uniform sampler2D samplers[3];

layout(location = 0) in vec2 st;
layout(location = 1) in vec3 dNormal;
layout(location = 2) in vec3 dLight;
layout(location = 3) in vec3 dCamera;


layout(location = 0) out vec4 outColor;

void main() {
    vec4 cDiffuse = texture(samplers[body.texIndices[0]], st);
    vec3 uNormal = normalize(dNormal);
    vec3 uLightPositional = normalize(dLight);
    float iDiffuse = max(0, dot(uNormal, vec3(scene.uLight)));
    float iDiffuse2 = max(0, dot(uNormal, uLightPositional));
    vec3 uCamera = normalize(dCamera);
    vec3 uReflection1 = ((2 * dot(vec3(scene.uLight), uNormal)) * vec3(scene.uLight)) - vec3(scene.uLight);
    vec3 uReflection2 = ((2 * dot(uLightPositional, uNormal)) * uLightPositional) - uLightPositional;
    float iSpec1 =  max(0, dot(uReflection1, uCamera));
    if(iDiffuse <= 0.0){
        iSpec1 = 0.0;
    }
    float iSpec2 =  max(0, dot(uReflection2, uCamera));
    if(iDiffuse2 <= 0.0){
        iSpec2 = 0.0;
    }
    vec3 pLightMinuspCamera = vec3(scene.pLight) - pCamera;
    float d = length(pLightMinuspCamera);
    float attenuation = 1/(1 + 0.5*d*d)
    outColor = (iDiffuse * scene.cLight * cDiffuse) + (iDiffuse2 * scene.cLight2 * cDiffuse * attenuation);
    outColor+= (scene.cAmbient * cDiffuse); 
    outColor+= pow(iSpec1, 64.0) * scene.cLight * body.cSpecular;
    outColor+=  pow(iSpec2, 64.0)  * scene.cLight2 * body.cSpecular;
}


