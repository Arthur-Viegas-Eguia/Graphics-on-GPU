


#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform SceneUniforms {
    mat4 camera;
    uvec4 uLight;
    uvec4 cLight;
    uvec4 pLight;
    uvec4 cLight2;
    uvec4 cAmbient;
    uvec4 pCamera;
} scene;
layout(binding = 1) uniform BodyUniforms {
    mat4 modeling;
    uvec4 texIndices;
    uvec4 cSpecular;
} body;

layout(location = 0) in vec3 attrXYZ;
layout(location = 1) in vec2 attrST;
layout(location = 2) in vec3 attrNOP;

layout(location = 0) out vec2 st;
layout(location = 1) out vec3 uNormal;
layout(location = 2) out vec3 uLight;
layout(location = 3) out vec3 uCamera;

void main() {
    vec4 pWorld = body.modeling * vec4(attrXYZ, 1.0);
    gl_Position = scene.camera * pWorld;
    vec3 uLightPreNormal = vec3((scene.pLight - pWorld));
    uLight = normalize(uLightPreNormal);
    st = attrST;
    uNormal = normalize(vec3(body.modeling * vec4(attrNOP, 0.0)));
    uCamera = normalize(vec3(scene.pCamera - pWorld));

}


