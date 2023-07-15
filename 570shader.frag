


#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform SceneUniforms {
    mat4 camera;
    vec4 uLight;
    vec4 cLight;
} scene;
layout(binding = 1) uniform BodyUniforms {
    mat4 modeling;
    uvec4 texIndices;
} body;
layout(binding = 2) uniform sampler2D samplers[3];

layout(location = 0) in vec2 st;
layout(location = 1) in vec4 dNormal;


layout(location = 0) out vec4 outColor;

void main() {
    vec4 cDiffuse = texture(samplers[body.texIndices[0]], st);
    vec4 uNormal = normalize(dNormal);
    float iDiffuse = max(0, dot(uNormal, scene.uLight));
    outColor = iDiffuse * scene.cLight * cDiffuse;
}


