


#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform SceneUniforms {
    mat4 camera;
    vec4 uLight;
    vec4 cLight;
    vec4 pLight;
    vec4 cLight2;
    vec4 cAmbient;
} scene;
layout(binding = 1) uniform BodyUniforms {
    mat4 modeling;
    uvec4 texIndices;
} body;
layout(binding = 2) uniform sampler2D samplers[3];

layout(location = 0) in vec2 st;
layout(location = 1) in vec3 dNormal;
layout(location = 2) in vec3 dLight;


layout(location = 0) out vec4 outColor;

void main() {
    vec4 cDiffuse = texture(samplers[body.texIndices[0]], st);
    vec3 uNormal = normalize(dNormal);
    vec3 uLightPositional = normalize(dLight);
    float iDiffuse = max(0, dot(uNormal, vec3(scene.uLight)));
    float iDiffuse2 = max(0, dot(uNormal, uLightPositional));
    outColor = (iDiffuse * scene.cLight * cDiffuse) + (iDiffuse2 * scene.cLight2 * cDiffuse) + (scene.cAmbient * cDiffuse);
}


