<!--
 * @Author: Your Name you@example.com
 * @Date: 2023-07-23 13:57:25
 * @LastEditors: Your Name you@example.com
 * @LastEditTime: 2023-07-23 13:59:11
 * @FilePath: \LearnOpenGLVscode\src\05_load_texture work\Readme.md
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
-->
## 纹理作业
-- 修改片段着色器，仅让笑脸图案朝另一个方向看
-- 能不能在箱子的角落放置4个笑脸 纹理环绕方式  纹理坐标 * 2
-- GL_REPEAT  纹理其余部分就是复制填充
-- GL_MIRRORED_REPEAT  纹理其余部分就是镜像复制填充

-- 尝试在矩形上只显示纹理图像的中间一部分，修改纹理坐标，达到能看见单个的像素的效果。尝试使用GL_NEAREST的纹理过滤方式让像素显示得更清晰： 只显示纹理一部分 () * 0.2f + 0.5f GL_NEAREST GL_LINEAR

-- mian 参数argv[0] argv[1] 0参数是./main.exe 1参数是dir=0xx