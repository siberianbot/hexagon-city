#version 460

layout (set = 0, binding = 0) uniform sampler2D albedoTextures[64];

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inColor;
layout (location = 3) in vec2 inUV;
flat layout (location = 4) in uint inTextureId;

layout (location = 0) out vec4 outAlbedo;

void main() {
    outAlbedo = vec4(inColor, 1) * texture(albedoTextures[inTextureId], inUV);
}
