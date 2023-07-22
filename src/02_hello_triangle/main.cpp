/*
 * @Author: Your Name you@example.com
 * @Date: 2023-07-09 18:12:08
 * @LastEditors: Your Name you@example.com
 * @LastEditTime: 2023-07-22 19:45:31
 * @FilePath: \LearnOpenGLVscode\src\01_hello_window\main.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const GLchar *vertexSharderSource = "#version 330 core\n"
                                    "layout (location = 0) in vec3 aPos;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
                                    "   gl_PointSize = 10.0f;\n"
                                    "}\n";

const GLchar *fragmentSharderSource = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0);\n"
                                    "}\n";

int main()
{
    glfwInit();
    // 设置主要版本和次要版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口对象
    GLFWwindow* window =  glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to creat GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // 设置视口 openGL渲染的窗口大小
    glViewport(0, 0, 800, 600);
    glEnable(GL_PROGRAM_POINT_SIZE);

    // 注册窗口大小变化监听函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 定义顶点数组
    GLfloat vertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };

    // 多个三角形很多重复节点，EBO 索引解决
    GLuint indices[] = {
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    // 创建顶点缓冲对象 VBO
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    // 绑定VAO对象
    glBindVertexArray(VAO);

    // 绑定缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    // 填充数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // 解释顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    // 解绑
    glBindVertexArray(0);

    // 创建顶点着色器、片段着色器
    GLuint vertexShader;
    GLuint fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // 附加着色器代码并编译
    glShaderSource(vertexShader, 1, &vertexSharderSource, NULL);
    glShaderSource(fragmentShader, 1, &fragmentSharderSource, NULL);
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
    // 检测是否编译成功
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FALIED\n" << infoLog 
                  << std::endl;
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FALIED\n" << infoLog 
                  << std::endl;
    }

    // 创建着色器程序对象
    GLint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FALIED\n" << infoLog 
                  << std::endl;
    }

    // 使用着色器程序
    glUseProgram(shaderProgram);

    // 删除着色器
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    

    while(!glfwWindowShouldClose(window)) {
        // 输入
        processInput(window);
        // 渲染
        glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_POINTS, 0, 6);
        //glDrawArrays(GL_LINE_LOOP, 0, 6);
        glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // 检查并调用事件，交换缓冲
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

/* 最后的初始化和绘制代码现在看起来像这样：
// ..:: 初始化代码 :: ..
// 1. 绑定顶点数组对象
glBindVertexArray(VAO);
// 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// 3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
// 4. 设定顶点属性指针
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

[...]

// ..:: 绘制代码（渲染循环中） :: ..
glUseProgram(shaderProgram);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
glBindVertexArray(0);
*/