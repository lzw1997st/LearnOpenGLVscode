#ifndef BUFFER_GEOMETRY
#define BUFFER_GEOMETRY

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <tool/Shader.h>

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
// GLM库
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vertex
{
    glm::vec3 Position;  // 顶点位置
    glm::vec3 Normal;    // 法线
    glm::vec2 TexCoords; // 纹理坐标
    glm::vec3 Tangent;   // 切线
    glm::vec3 Bitangent; // 副切线
};

class BufferGeometry
{
private:
    /* data */
    // 单位矩阵
    // glm::mat4 E(1.0f);
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    unsigned int VAO;
    unsigned int ID;

    void debugInfo()
    {
        std::cout << "-----------Position----------" << std::endl;
        for (auto &p : vertices)
        {
            std::cout << "[";
            std::cout << std::setw(5) << p.Position.x << ",";
            std::cout << std::setw(5) << p.Position.y << ",";
            std::cout << std::setw(5) << p.Position.z << "]" << std::endl;
        }
        std::cout << "-----------Position----------" << std::endl;
        std::cout << "-----------Index-------------" << std::endl;
        for (int i = 0; i < indices.size() / 3; i++)
        {
            std::cout << "[";
            for (int j = 0; j < 3; j++)
            {
                std::cout << std::setw(5) << indices[i * 3 + j] << ",";
            }
            std::cout << "]" << std::endl;
        }
        std::cout << "-----------Index-------------" << std::endl;
    }

    void destory()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteVertexArrays(1, &this->VAO);
        glDeleteBuffers(1, &this->VBO);
        glDeleteBuffers(1, &this->EBO);
    }

protected:
    unsigned int VBO, EBO;

    void creatBuffers(void)
    {
        glGenBuffers(1, &this->VBO);
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->EBO);
        // 绑定VAO对象
        glBindVertexArray(this->VAO);
        // 顶点坐标
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        // 下标
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        // 设置属性指针
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        glEnableVertexAttribArray(0);
        // Normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
        glEnableVertexAttribArray(1);
        // Texcoords
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));
        glEnableVertexAttribArray(2);
        // 解绑
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
};

#endif