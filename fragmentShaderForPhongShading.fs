#version 330 core
out vec4 FragColor;

struct Material {   //for object
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    vec3 emissive;
};



struct PointLight {
    vec3 position;    
    float k_c;  // attenuation factors
    float k_l;  // attenuation factors
    float k_q;  // attenuation factors
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct SpotLight {
    vec3 position;
    vec3 direction;
    float cos_theta;    
    float k_c;  // attenuation factors
    float k_l;  // attenuation factors
    float k_q;  // attenuation factors    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct DirectionalLight {
    vec3 direction;    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


#define NR_POINT_LIGHTS 3
#define NR_DIR_LIGHTS 2
#define NR_SPOT_LIGHTS 2

in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform Material material;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirectionalLight directionalLight[NR_DIR_LIGHTS];
uniform SpotLight spotLight[NR_SPOT_LIGHTS];


// function prototypes
vec3 CalcPointLight(Material material, PointLight light, vec3 N, vec3 fragPos, vec3 V); //N for Normal, V for viewing
vec3 CalcDirectionalLight(Material material, DirectionalLight light, vec3 N, vec3 V);
vec3 CalcSpotLight(Material material, SpotLight light, vec3 N, vec3 fragPos, vec3 V);


void main()
{
    // properties
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);
    
    vec3 result;
    // point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(material, pointLights[i], N, FragPos, V);
    for(int i = 0; i < NR_DIR_LIGHTS; i++)
         result += CalcDirectionalLight(material, directionalLight[i], N, V);
    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
        result += CalcSpotLight(material, spotLight[i], N, FragPos, V);
        
    FragColor = vec4(result, 1.0);
}

// calculates the color when using a point light.
vec3 CalcPointLight(Material material, PointLight light, vec3 N, vec3 fragPos, vec3 V)
{
    vec3 L = normalize(light.position - fragPos);
    vec3 R = reflect(-L, N);//specular
    
    vec3 K_A = material.ambient;
    vec3 K_D = material.diffuse;
    vec3 K_S = material.specular;
    vec3 K_E = material.emissive;
    
    // attenuation
    float d = length(light.position - fragPos);
    float attenuation = 1.0 / (light.k_c + light.k_l * d + light.k_q * (d * d));
    
    vec3 ambient = K_A * light.ambient;
    vec3 diffuse = K_D * max(dot(N, L), 0.0) * light.diffuse; //max to avoid negative value, 0 to 1
    vec3 specular = K_S * pow(max(dot(V, R), 0.0), material.shininess) * light.specular;
    vec3 emissive = K_E;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular + emissive);
}


vec3 CalcDirectionalLight(Material material, DirectionalLight light, vec3 N, vec3 V)
{
    vec3 L = normalize(-light.direction);
    vec3 R = reflect(-L, N);//specular
    
    vec3 K_A = material.ambient;
    vec3 K_D = material.diffuse;
    vec3 K_S = material.specular;
    vec3 K_E = material.emissive;
    
    
    vec3 ambient = K_A * light.ambient;
    vec3 diffuse = K_D * max(dot(N, L), 0.0) * light.diffuse; //max to avoid negative value, 0 to 1
    vec3 specular = K_S * pow(max(dot(V, R), 0.0), material.shininess) * light.specular;
    vec3 emissive = K_E;
    
    
    return (ambient + diffuse + specular + emissive);
}

vec3 CalcSpotLight(Material material, SpotLight light, vec3 N, vec3 fragPos, vec3 V)
{
    vec3 L = normalize(light.position - fragPos);
    vec3 R = reflect(-L, N);//specular
    
    vec3 K_A = material.ambient;
    vec3 K_D = material.diffuse;
    vec3 K_S = material.specular;
    vec3 K_E = material.emissive;
    
    // attenuation
    float d = length(light.position - fragPos);
    float attenuation = 1.0 / (light.k_c + light.k_l * d + light.k_q * (d * d));
    
    vec3 ambient = K_A * light.ambient;
    vec3 diffuse = K_D * max(dot(N, L), 0.0) * light.diffuse; //max to avoid negative value, 0 to 1
    vec3 specular = K_S * pow(max(dot(V, R), 0.0), material.shininess) * light.specular;
    vec3 emissive = K_E;

    float cos_gamma = dot(L, normalize(-light.direction));
    if(cos_gamma < light.cos_theta)
    {
        cos_gamma = 0;
    }
    
    ambient *= attenuation * cos_gamma;
    diffuse *= attenuation * cos_gamma;
    specular *= attenuation * cos_gamma;
    
    return (ambient + diffuse + specular + emissive);
}