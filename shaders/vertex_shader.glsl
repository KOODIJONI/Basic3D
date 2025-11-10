#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent; // required for proper normal mapping

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec2 fragTexCoord;
out mat3 TBN;

void main()
{
    vec4 worldPos = uModel * vec4(aPos, 1.0);
    fragTexCoord = aTexCoord;
    gl_Position = uProjection * uView * worldPos;

    // Transform normals/tangents to world space
    vec3 normalWorld = normalize(mat3(uModel) * aNormal);
    vec3 tangentWorld = normalize(mat3(uModel) * aTangent);
    vec3 bitangentWorld = normalize(cross(normalWorld, tangentWorld));

    TBN = mat3(tangentWorld, bitangentWorld, normalWorld);
}
