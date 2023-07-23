/*
 * @Author: Your Name you@example.com
 * @Date: 2023-07-09 18:12:08
 * @LastEditors: Your Name you@example.com
 * @LastEditTime: 2023-07-23 10:36:00
 * @FilePath: \LearnOpenGLVscode\src\01_hello_window\main.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <tool/Shader.h>
#include <cmath>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

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

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    // 注册窗口大小变化监听函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Shader ourShader1("./src/04_shader_class/shader/vertex.glsl", "./src/04_shader_class/shader/fragment.glsl");
    Shader ourShader2("./src/04_shader_class/shader/vertex.glsl", "./src/04_shader_class/shader/fragment.glsl");


    // 定义顶点数组
    GLfloat vertices1[] = {
         0.5f,  0.5f, 0.0f,   // 右上角
         0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f,  0.5f, 0.0f   // 左上角
    };

    // 多个三角形很多重复节点，EBO 索引解决
    GLuint indices1[] = {
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };

    GLfloat vertices2[] = {
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };

    // 多个三角形很多重复节点，EBO 索引解决
    GLuint indices2[] = {
        0, 1, 2, // 第一个三角形
    };

    // 创建顶点缓冲对象 VBO
    GLuint VBOs[2];
    GLuint VAOs[2];
    GLuint EBOs[2];
    glGenBuffers(2, VBOs);
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, EBOs);
    // 绑定VAO对象
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    float xOffset = 0.3f;
 
    while(!glfwWindowShouldClose(window)) {
        // 输入
        processInput(window);
        // 渲染
        glClearColor(0.0f, 0.1f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ourShader1.use();
        // glBindVertexArray(VAOs[0]);
        // glDrawArrays(GL_POINTS, 0, 6);
        // glDrawArrays(GL_LINE_LOOP, 0, 6);
        // glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);

        ourShader2.use();
        ourShader2.setFloat("xOffset", xOffset);
        float time = glfwGetTime();
        float red = sin(time) / 2.0f + 0.5f;
        glUniform4f(glGetUniformLocation(ourShader2.ID, "ourColor"), red, 0.0f, 0.0f, 1.0f);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // 检查并调用事件，交换缓冲
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
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