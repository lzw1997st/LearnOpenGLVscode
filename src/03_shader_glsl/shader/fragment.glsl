#version 330 core
out vec4 FragColor;
uniform vec4 ourColor;
void main()
{
   vec4 color = vec4(0.0f, 0.0f, 1.0f, 1.0);
   FragColor = ourColor;
}
