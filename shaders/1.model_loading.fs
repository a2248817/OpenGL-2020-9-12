#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 fragPos;

uniform vec3 viewPos;
uniform vec3 viewDir;
uniform vec4 lightColor;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

uniform Material material;

float attenuation();

float intensity();


void main()
{   
    vec4 objColor =  texture(material.texture_diffuse1, TexCoords);
    vec3 diffColor = vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specColor = vec3(texture(material.texture_specular1, TexCoords));

    vec3 norm = normalize(Normal);
    vec3 ambient = diffColor * 0.1;
    
    vec3 lightDir = normalize(-viewDir);
    float diff = max(dot(lightDir, norm), 0.0);

    float spec = max(dot(reflect(-lightDir, norm), normalize(viewPos - fragPos)), 0.0);

    float atte = attenuation();
    float inten = intensity();
    
    vec3 diffuse  = diffColor * diff * atte * 0.5 * inten * vec3(lightColor);
    vec3 specular = specColor * spec * atte       * inten * vec3(lightColor);
    
    FragColor = objColor * vec4((ambient + diffuse + specular), 1.0);
}

float attenuation()
{
    float constant = 1.0;
    float linear = 0.09;
    float quadratic = 0.032;
    float distance = length(viewPos - fragPos);
    return 1.0 / (constant * distance + linear * distance + quadratic * distance * distance);
}

float intensity()
{
    float innerCutOff = cos(radians(10.0));
    float outerCutOff = cos(radians(12.5));
    vec3 objDir = normalize(fragPos - viewPos);
    return clamp((dot(objDir, viewDir) - outerCutOff) / (innerCutOff - outerCutOff), 0.0, 1.0);
}