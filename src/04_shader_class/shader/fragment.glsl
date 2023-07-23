#version 330 core
out vec4 FragColor;

uniform vec4 ourColor;
in vec3 ourPos;

void main()
{
   vec4 color = vec4(ourPos, 1.0f);
   FragColor = color;
}
