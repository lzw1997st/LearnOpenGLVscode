#version 330 core
out vec4 FragColor;
in vec2 outTexCoord;
in vec3 outNormal;
in vec3 outFragPos;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float ambientStrengh;
uniform vec3 viewPos; // 相机位置

uniform sampler2D texture1;
uniform sampler2D texture2;

// 定义材质结构体
struct Material {
    sampler2D  diffuse; // 漫反射光照颜色
    sampler2D  specular; // 镜面反射光照颜色
    float shininess;  //镜面反射光(高光)指数
}; 
uniform Material material;

// 定向光
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;

// 点光源
struct PointLight {
    vec3 position;

    float constant; // 点光源衰减
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

// 手电筒
struct SpotLight {
    vec3  position;
    vec3  direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform SpotLight spotLight;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
  // 环境光照
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, outTexCoord));
  // 漫反射光照
  vec3 lightDir = normalize(-light.direction);
  float diff = max(dot(normal, lightDir), 0.0f);
  vec3 diffuse = light.diffuse * (vec3(texture(material.diffuse, outTexCoord)) * diff);
  // 镜面光照
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
  vec3 speccular = light.specular * (spec * vec3(texture(material.specular, outTexCoord)));

  // 最终光照结果
  return ambient + diffuse + speccular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
  // 点光源衰减
  float distance    = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + 
                light.quadratic * (distance * distance));

  // 环境光照
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, outTexCoord));
  // 漫反射光照
  vec3 norm = normalize(outNormal);
  vec3 lightDir = normalize(light.position - fragPos);

  float diff = max(dot(norm, lightDir), 0.0f);
  vec3 diffuse = light.diffuse * (vec3(texture(material.diffuse, outTexCoord)) * diff);
  // 镜面光照
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
  vec3 speccular = light.specular * (spec * vec3(texture(material.specular, outTexCoord)));
  // 最终光照结果
  return attenuation * (ambient + diffuse + speccular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
  // 从片段指向光源的向量
  vec3 lightDir = normalize(light.position - fragPos);
  // 聚光灯衰减
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon   = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);  

  if(theta > light.cutOff) 
  {       
    // 执行光照计算
    // 点光源衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                  light.quadratic * (distance * distance));
    // 环境光照
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, outTexCoord));
    // 漫反射光照
    vec3 norm = normalize(outNormal);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * (vec3(texture(material.diffuse, outTexCoord)) * diff);
    // 镜面光照
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 speccular = light.specular * (spec * vec3(texture(material.specular, outTexCoord)));
    // 最终光照结果
    return attenuation * intensity * (ambient + diffuse + speccular);
  }

  // 否则，使用环境光，让场景在聚光之外时不至于完全黑暗
  return light.ambient;
}

void main() {

  // vec4 objectColor = mix(texture(texture1, outTexCoord), texture(texture2, outTexCoord), 0.1);
  vec4 objectColor = vec4(1.0f, 1.0f,1.0f, 1.0f);

  // 属性
  vec3 norm = normalize(outNormal);
  vec3 viewDir = normalize(viewPos - outFragPos);

  // 第一阶段：定向光照
  vec3 result;
  result += CalcDirLight(dirLight, norm, viewDir);
  // // 第二阶段：点光源
  for(int i = 0; i < NR_POINT_LIGHTS; i++)
      result += CalcPointLight(pointLights[i], norm, outFragPos, viewDir);    
  // 第三阶段：聚光
  result += CalcSpotLight(spotLight, norm, outFragPos, viewDir);    

  FragColor = vec4(result, 1.0) * objectColor;
}