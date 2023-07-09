1. 下载[MinGW](https://sourceforge.net/projects/mingw/)，安装gcc、g++、gdb、进入到MInGW\bin下将mingw-32-make.exe 复制一份并改名为make.exe

2. vscode 插件 `C/C++`、`C/C++ Project Generator`

   1. ctrl+shift+p输入creat创建C++项目

   2. 创建好了后，终端输入make run,确保hello ward

   3. c_cpp_properties.json 包含路径要加入MinGW/bin, make使用的是g++

      ```
      {
          "configurations": [
              {
                  "name": "Win32",
                  "includePath": [
                      "D:/MinGW/bin/**",
                      "${workspaceFolder}/**"
                  ],
                  "defines": [
                      "_DEBUG",
                      "UNICODE",
                      "_UNICODE"
                  ],
                  "compilerPath": "D:/MinGW/bin/g++.exe",
                  "cStandard": "c17",
                  "cppStandard": "c++17",
                  "intelliSenseMode": "gcc-x86"
              }
          ],
          "version": 4
      }
      ```

      

3. glfw 下载 [`Windows pre-compiled binaries`](https://www.glfw.org/download.html)

   1. 选择**Windows pre-compiled binaries**，因为我们使用的 MinGW 所以选择 [32-bit Windows binaries](https://github.com/glfw/glfw/releases/download/3.3.4/glfw-3.3.4.bin.WIN32.zip) 对于 Ubuntu，通过 `sudo apt install libglfw3-dev libglfw3` 安装 glfw
   2. glfw-3.3.8.bin.WIN32\include\GLFW 文件夹复制到vscode项目include中
   3. glfw-3.3.8.bin.WIN32\lib-mingw\glfw3.dll 文件复制到vscode项目output中，因为openGL输出需要这个dll文件

4. glad [在线服务](https://glad.dav1d.de/) 生成静态库

   1. ![](C:\Users\lzw\AppData\Roaming\Typora\typora-user-images\image-20230709161116307.png)

   2.![](C:\Users\lzw\AppData\Roaming\Typora\typora-user-images\image-20230709161230811.png)

   3. ```
      > gcc ./src/glad.c -c -I ./include/ // 生成 .o文件
      > ar -rc libglad.a glad.o           // 生成我们所需要的 .a文件
      ```

      > 将生成的 libglad.a 复制到 lib 文件下
      >
      > 将inlcude下的glad、KHR文件夹复制到 include 文件夹下

5. 修改makefile

   1. makefile 中新增lib库

      ```
      # define library directory
      LIBRARY_DIR			:=	lib
      Libraries			:=  -lglad -lglfw3dll
      ```

   2. 在build 是加入这个lib

      ```
      $(MAIN): $(OBJECTS)
      	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS) $(LIBS) $(Libraries)
      ```

6. 测试

   make run/make clean

   1. 测试代码

   ```
   #include <glad/glad.h>
   #include <GLFW/glfw3.h>
   
   #include <iostream>
   
   void framebuffer_size_callback(GLFWwindow *window, int width, int height);
   void processInput(GLFWwindow *window);
   
   const unsigned int SCR_WIDTH = 800;
   const unsigned int SCR_HEIGHT = 600;
   
   int main()
   {
   	glfwInit();
   	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   
   	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
   	if (window == NULL)
   	{
   		std::cout << "Failed to create GLFW window" << std::endl;
   		glfwTerminate();
   		return -1;
   	}
   	glfwMakeContextCurrent(window);
   	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
   
   	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
   	{
   		std::cout << "Failed to initialize GLAD" << std::endl;
   		return -1;
   	}
   
   	while (!glfwWindowShouldClose(window))
   	{
   		processInput(window);
   		glfwSwapBuffers(window);
   		glfwPollEvents();
   	}
   
   	glfwTerminate();
   	return 0;
   }
   
   void processInput(GLFWwindow *window)
   {
   	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
   		glfwSetWindowShouldClose(window, true);
   }
   
   void framebuffer_size_callback(GLFWwindow *window, int width, int height)
   {
   	glViewport(0, 0, width, height);
   }
   ```

   2. DEBUG

      需要设置调试一个文件 修改 `tasks.json`下面的条目

      ```
       "label": "build",
        "type": "shell",
        "group": {
            "kind": "build",
            "isDefault": true
        },
        "windows": {
            "command": "powershell",
            "args": [
                "-c",
                "make dir=01_test_glfw", // 调式 01_test_glfw/main.cpp
            ]
        },
      ```

      