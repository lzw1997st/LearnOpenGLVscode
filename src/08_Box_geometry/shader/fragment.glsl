#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
   FragColor = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2f);
   // FragColor = texture(ourTexture2, TexCoord);
   // FragColor =vec4(0.0549, 0.0, 0.5569, 1.0);
}
