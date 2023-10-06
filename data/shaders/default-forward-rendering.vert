#version 460

layout (push_constant) uniform PerRenderData {
    mat4 projectionView;
} perRenderData;

// per instance
layout (location = 0) in mat4 instanceModel;
layout (location = 4) in mat4 instanceModelRot;
layout (location = 8) in vec3 instanceColor;
layout (location = 9) in uint instanceTextureId;

// per vertex
layout (location = 10) in vec3 vertexPos;
layout (location = 11) in vec3 vertexNormal;
layout (location = 12) in vec3 vertexColor;
layout (location = 13) in vec2 vertexUV;

layout (location = 0) out vec3 outPos;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec3 outColor;
layout (location = 3) out vec2 outUV;
flat layout (location = 4) out uint outTextureId;

void main() {
    gl_Position = perRenderData.projectionView * instanceModel * vec4(vertexPos, 1);

    outPos = (instanceModel * vec4(vertexPos, 1)).xyz;
    outNormal = (instanceModelRot * vec4(vertexNormal, 1)).xyz;
    outColor = instanceColor * vertexColor;
    outUV = vertexUV;
}
