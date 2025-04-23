#version 330 core

out vec4 fragColor;

struct LightBase
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D uTextureSampler;
uniform LightBase uLightBase;
uniform vec3 uLightColor;
uniform vec3 uViewPos;

in vec2 TextureCoord;
in vec3 FragPos;
in vec3 Normal;

void main() 
{ 
    vec3 normalNormalize = normalize(Normal);
    vec3 lightDirection = normalize(-uLightBase.direction);
    float diffuseTemp = max(dot(normalNormalize, lightDirection), 0.0);
    vec3 diffuse = uLightBase.diffuse * diffuseTemp * uLightColor;
    vec3 ambient = uLightBase.ambient * uLightColor;

    vec3 viewDirection = normalize(uViewPos - FragPos);
    vec3 reflectDirection = reflect(-lightDirection, normalNormalize);
    float specularTemp = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    vec3 specular = uLightBase.specular * specularTemp * uLightColor;

    fragColor = vec4(ambient + diffuse + specular, 1.0) * texture(uTextureSampler, TextureCoord);
}