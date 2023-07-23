/*
 * @Author: Your Name you@example.com
 * @Date: 2023-07-23 13:57:25
 * @LastEditors: Your Name you@example.com
 * @LastEditTime: 2023-07-23 14:07:26
 * @FilePath: \LearnOpenGLVscode\src\05_load_texture copy\shader\fragment.glsl
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

uniform float factor;

void main()
{
   // FragColor = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), factor);
   FragColor = texture(ourTexture2, TexCoord);
}
