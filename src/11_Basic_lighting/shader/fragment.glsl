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

void main() {

  vec4 objectColor = mix(texture(texture1, outTexCoord), texture(texture2, outTexCoord), 0.1);
  // 环境光照
  vec3 ambient = ambientStrengh * lightColor;
  // 漫反射光照
  vec3 norm = normalize(outNormal);
  vec3 lightDir = normalize(lightPos - outFragPos);

  float diff = max(dot(norm, lightDir), 0.0f);
  vec3 diffuse = diff * lightColor;
  // 镜面光照
  vec3 viewDir = normalize(viewPos - outFragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
  float specularStrength = 0.5;
  vec3 speccular = spec * lightColor * specularStrength;
  // 最终光照结果
  vec3 result = (ambient + diffuse + speccular) * vec3(objectColor);

  FragColor = vec4(result, 1.0);
}