#version 330 core
out vec4 fragColor;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
#define NR_POINT_LIGHTS 4

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

uniform vec3 viewPos;

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;


void main()
{
	
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(viewPos - fragPos);


	vec3 result = CalcDirLight(dirLight, norm, viewDir);

	for(int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);
	}
	result += CalcSpotLight(spotLight, norm, fragPos, viewDir);

	fragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.diffuse, texCoords));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return(ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 ambient = light.ambient * texture(material.diffuse, texCoords).rgb;

	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texCoords).rgb;
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, texCoords).rgb;

	
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = (light.cutOff - light.outerCutoff);
	float intensity = clamp((theta - light.outerCutoff) / epsilon , 0.0, 1.0);
	diffuse *= intensity;
	specular *= intensity;

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;

	return(ambient + diffuse + specular);
}
