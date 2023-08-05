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
    vec3 ambient; // 环境光照颜色
    vec3 diffuse; // 漫反射光照颜色
    vec3 specular; // 镜面反射光照颜色
    float shininess;  //镜面反射光(高光)指数
}; 

uniform Material material;

// 定义光的属性结构体
struct Light {
    vec3 position;  
    vec3 ambient;   // 光源环境光分量
    vec3 diffuse;   // 光源漫反射分量
    vec3 specular;  // 光源镜面光分量
};

uniform Light light;

void main() {

  // vec4 objectColor = mix(texture(texture1, outTexCoord), texture(texture2, outTexCoord), 0.1);
  vec4 objectColor = vec4(1.0f, 1.0f,1.0f, 1.0f);

  // 环境光照
  vec3 ambient = light.ambient * material.ambient;
  // 漫反射光照
  vec3 norm = normalize(outNormal);
  vec3 lightDir = normalize(lightPos - outFragPos);

  float diff = max(dot(norm, lightDir), 0.0f);
  vec3 diffuse = light.diffuse * (material.diffuse * diff);
  // 镜面光照
  vec3 viewDir = normalize(viewPos - outFragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
  vec3 speccular = light.specular * (spec * material.specular);
  // 最终光照结果
  vec3 result = (ambient + diffuse + speccular) * vec3(objectColor);

  FragColor = vec4(result, 1.0);
}