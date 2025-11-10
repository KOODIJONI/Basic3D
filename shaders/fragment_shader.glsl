#version 330 core

in vec2 fragTexCoord;
in mat3 TBN;

uniform sampler2D uTexture;       // Albedo / base color
uniform sampler2D uNormalMap;     // Normal map
uniform sampler2D uRoughnessMap;  // Roughness map
uniform sampler2D uMetalnessMap;  // Metalness map
uniform sampler2D uAOMap;         // Ambient Occlusion map

uniform bool uCastsShadows;

out vec4 FragColor;

// Hardcoded directional light
vec3 lightDir = normalize(vec3(-1.0, 1.0, -1.0));
vec3 lightColor = vec3(1.0);

float LightStrength  = 5.0;
float AmbientStrength = 0.3;
const float PI = 3.14159265359;

// Helper functions f float uLightStrength;or PBR
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / max(denom, 0.0001);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
    if(!uCastsShadows){
        FragColor = texture(uTexture, fragTexCoord);
        return;
    }
    // Sample textures
    vec3 albedo     = pow(texture(uTexture, fragTexCoord).rgb, vec3(2.2)); // gamma correction
    vec3 tangentNormal = texture(uNormalMap, fragTexCoord).rgb;
    tangentNormal = normalize(tangentNormal * 2.0 - 1.0);
    vec3 N = normalize(TBN * tangentNormal);

    float roughness = texture(uRoughnessMap, fragTexCoord).r;
    float metallic  = texture(uMetalnessMap, fragTexCoord).r;
    float ao        = texture(uAOMap, fragTexCoord).r;

    // View vector
    vec3 V = normalize(vec3(0.0, 0.0, 1.0)); // camera pointing along +Z
    vec3 L = normalize(lightDir);
    vec3 H = normalize(V + L);

    // Calculate PBR terms
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    float NDF = DistributionGGX(N, H, roughness);
    float G   = GeometrySmith(N, V, L, roughness);
    vec3 F    = FresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 nominator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular = nominator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    float NdotL = max(dot(N, L), 0.0);

    vec3 ambient = AmbientStrength * albedo * ao;  // AO affects ambient
    vec3 diffuse = kD * albedo / PI;
    vec3 color = ambient + (diffuse + specular) * NdotL * lightColor * LightStrength;


    // Gamma correction
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}
