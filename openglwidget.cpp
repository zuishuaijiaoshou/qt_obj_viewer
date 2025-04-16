#include "../3rdparty/tiny_obj_loader.h"
#include "../3rdparty/glm/gtc/matrix_transform.hpp"
#include <QMessageBox>
#include <QString>
#include <string>

#include "openglwidget.h"

// 顶点着色器源码
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

// 片段着色器源码
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(0.0, 1.0, 1.0, 1.0); // 青色
}
)";


void OpenGLWidget::RenderObjFile() {
    // 设置视口
    glViewport(0, 0, 800, 600);

    // 编译顶点着色器
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // 检查顶点着色器编译错误
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        QString error_msg = "Vertex shader compilation failed: " + QString(infoLog);
        QMessageBox::critical(this, "", error_msg);
    }

    // 编译片段着色器
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // 检查片段着色器编译错误
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        QString error_msg = "Fragment shader compilation failed: " + QString(infoLog);
        QMessageBox::critical(this, "", error_msg);
    }

    // 链接着色器程序
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 检查链接错误
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        QString error_msg = "Shader program linking failed: " + QString(infoLog);
        QMessageBox::critical(this, "", error_msg);
    }

    // 删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 加载 .obj 文件
    std::vector<glm::vec3> vertices;
    if (LoadObj(vertices)) {
        QMessageBox::critical(this, "", "load obj file fail");
    }

    // 创建 VAO 和 VBO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 绑定 VAO
    glBindVertexArray(VAO);

    // 绑定 VBO 并填充数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 解绑 VAO 和 VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 启用深度测试
    glEnable(GL_DEPTH_TEST);

    // 主循环
    CirclePaint(shaderProgram, vertices, VAO);

    // 清理资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

}

void OpenGLWidget::CirclePaint(GLuint shaderProgram, const std::vector<glm::vec3>& vertices, GLuint VAO) {

    // 清空颜色缓冲和深度缓冲
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 使用着色器程序
    glUseProgram(shaderProgram);

    // 设置模型、视图和投影矩阵
    glm::mat4 model = glm::mat4(1.0f);

    // rotate with z axis
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 view = glm::lookAt(glm::vec3(2.5f, 2.5f, 2.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // 传递矩阵到着色器
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

    // 绑定 VAO 并绘制
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

}

bool OpenGLWidget::LoadObj(std::vector<glm::vec3>& out_vertices) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file_nm.toUtf8().constData())) {
        QString error_msg = "Error loading OBJ file: " + QString(warn.c_str()) + QString(err.c_str());
        QMessageBox::critical(this, "", error_msg);
        return false;
    }

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            glm::vec3 vertex = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };
            out_vertices.push_back(vertex);
        }
    }

    return true;
}


