#pragma once
#include <glm/ext/matrix_transform.hpp>

#include "shader.h"
#define M_PI 3.14159265358979323846264338327950288

struct Material
{
public:
    glm::vec4 color;
    unsigned int textureID;
    bool isTextured;
    Shader shader;
    Material()
    {
        color = glm::vec4(1, 1, 1, 1);
        isTextured = false;
    }

    Material(glm::vec4 _color)
    {
        color = _color;
        isTextured = false;
    }
    Material(unsigned int _textureID)
    {
        textureID = _textureID;
        isTextured = true;
    }

    void setShader(Shader s)
    {
        shader = s;
    }

    void setTexture(unsigned int _textureID)
    {
        textureID = _textureID;
        isTextured = true;
    }
};



struct ObjectTransform
{
    glm::vec3 translation;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::mat4 model;
    ObjectTransform()
    {
        translation = glm::vec3(0, 0, -7);
        rotation = glm::vec3(0, 0, 0);
        scale = glm::vec3(1, 1, 1);
        CalculateModel();
    }

    ObjectTransform(glm::vec3 _translate, glm::vec3 _rotation, glm::vec3 _scale)
    {
        translation = _translate;
        rotation = _rotation;
        scale = _scale;
        CalculateModel();
    }

    void CalculateModel()
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, translation);

        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        model = glm::scale(model, scale);
    }


};

class ObjectRenderData
{
public:
    Material material;
    unsigned int VAO;
    unsigned int VBO;
    std::vector<float> vertices;
    virtual void VBOInit() {}
    virtual void VAOInit() {}
    virtual void VerticesInit() {}
    virtual void draw() {}
    virtual void deInitialize() {}
    bool isSkyBox = false;

    void SetMaterial(Material mat)
    {
        material = mat;
    }

};

class Cube :public ObjectRenderData
{
public:


    Cube()
    {
        VerticesInit();
        VBOInit();
        VAOInit();
    }

    void VerticesInit() override
    {
        vertices = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };
    }

    void VBOInit() override
    {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    }

    void VAOInit() override
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);
    }

    void deInitialize() override
    {
        glDeleteVertexArrays(1, &VAO);
    }

    void draw() override
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }
};


class Plane : public ObjectRenderData
{
public:

    Plane()
    {
        VerticesInit();
        VBOInit();
        VAOInit();
    }


    void VerticesInit() override
    {
        vertices =
        {
             -1.0, 0.0, 1.0, 0.0, 0.0,
            1.0, 0.0, 1.0, 1.0, 0.0,
            -1.0, 0.0, -1.0, 0.0, 1.0,
            1.0, 0.0, -1.0, 1.0, 1.0
        };
    }

    void VBOInit() override
    {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    }

    void VAOInit() override
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);
    }

    void deInitialize() override
    {
        glDeleteVertexArrays(1, &VAO);
    }

    void draw() override
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

    }
};


class Sphere :public ObjectRenderData
{
private:
    int numSlices = 100;
    int numStacks = 5;
    float radius = 1.0;
public:

    Sphere()
    {
        VerticesInit();
        VBOInit();
        VAOInit();

    }

    void VerticesInit() override
    {
        for (int i = 0; i < numSlices; ++i) {
            double lat0 = M_PI * (-0.5 + (double)(i) / numSlices);
            double z0 = sin(lat0);
            double zr0 = cos(lat0);

            double lat1 = M_PI * (-0.5 + (double)(i + 1) / numSlices);
            double z1 = sin(lat1);
            double zr1 = cos(lat1);

            for (int j = 0; j <= numStacks; ++j) {
                double lng = 2 * M_PI * (double)(j - 1) / numStacks;
                double x = cos(lng);
                double y = sin(lng);

                vertices.push_back(x * zr0 * radius);
                vertices.push_back(y * zr0 * radius);
                vertices.push_back(z0 * radius);

                vertices.push_back(x * zr1 * radius);
                vertices.push_back(y * zr1 * radius);
                vertices.push_back(z1 * radius);
            }
        }
    }

    void VBOInit() override
    {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    }

    void VAOInit() override
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindVertexArray(0);
    }

    void deInitialize() override
    {
        glDeleteVertexArrays(1, &VAO);
    }

    void draw() override
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, numSlices * (numStacks + 1) * 2);
        glBindVertexArray(0);

    }
};



class ScreenQuad : public ObjectRenderData
{
public:

    ScreenQuad()
    {
        VerticesInit();
        VBOInit();
        VAOInit();

    }

    void VerticesInit() override
    {
        vertices =
        {
           -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
        };
    }

    void VBOInit() override
    {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    }

    void VAOInit() override
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);
    }

    void deInitialize() override
    {
        glDeleteVertexArrays(1, &VAO);
    }

    void draw() override
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

    }
};

