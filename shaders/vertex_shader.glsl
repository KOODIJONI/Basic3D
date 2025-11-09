#version 330 core

layout(location = 0) in vec3 aPos;      // vertex position
layout(location = 1) in vec3 aNormal;   // vertex normal
layout(location = 2) in vec2 aTexCoord; // texture coordinates

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

uniform vec3 uLightDir; // directional light, normalized in C code

out vec2 fragTexCoord;
out float lightIntensity;

void main()
{
    // World-space position
    vec4 worldPos = uModel * vec4(aPos, 1.0);
    gl_Position = uProjection * uView * worldPos;

    // Transform normal to world space
    mat3 normalMatrix = transpose(inverse(mat3(uModel)));
    vec3 normalWorld = normalize(normalMatrix * aNormal);

    // Lambertian diffuse
    lightIntensity = max(dot(normalWorld, normalize(vec3(3.0,3.0,3.0))), 0.0);

    fragTexCoord = aTexCoord;
}
