#pragma once
#include <string>

#include "shader.h"



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
        vertices =
        {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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

#include<filesystem>
// Function to get the path to the current executable
inline std::wstring GetExePath() {
    wchar_t buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    return std::wstring(buffer).substr(0, pos);
}

class GameObject
{
public:
    std::shared_ptr<ObjectRenderData> renderData;//stores
    std::string name;//name of the game object
    ObjectTransform transform;//transform data of the gameobject

    GameObject() {
        // Set default values or perform initialization if needed
        name = "nothing selected";
    }

	GameObject(const std::string name) : name(name)//test constructor
	{
        //std::filesystem::current_path("C://Users//sachi//OneDrive//Documents//GitHub//Game Engine//PlipPlop//NarrativeEngine//NarrativeEngine//Shaders"); //setting path


        //std::filesystem::path exePath = std::filesystem::absolute(std::filesystem::path(argv[0]));
        //std::filesystem::path shadersPath = exePath.parent_path() / "Shaders"; // Assuming Shaders folder is in the same directory as the executable

        std::wstring exePath = GetExePath();
        std::filesystem::path shadersPath = std::filesystem::path(exePath) / "Shaders"; // Assuming Shaders folder is in the same directory as the executable

        // Set the path to the shaders
        std::filesystem::current_path(shadersPath);


    	auto path = std::filesystem::current_path();

        std::cout << path;
        renderData = std::make_shared<Cube>();// new Plane();
        const Material mat(glm::vec4(1, 1, 1, 1));
        renderData->SetMaterial(mat);
          Shader s("defaultShader.vert","defaultShader.frag");
          renderData->material.setShader(s);

        std::cout << "\n GAME OBJECT CREATED \n";
       
	}

    GameObject(const std::string name, ObjectTransform transformData):name(name),transform(transformData)
    {
       // std::filesystem::current_path("C://Users//sachi//OneDrive//Documents//GitHub//Game Engine//PlipPlop//NarrativeEngine//NarrativeEngine//Shaders"); //setting path
       // auto path = std::filesystem::current_path();
        std::wstring exePath = GetExePath();
        std::filesystem::path shadersPath = std::filesystem::path(exePath) / "Shaders"; // Assuming Shaders folder is in the same directory as the executable

        // Set the path to the shaders
        std::filesystem::current_path(shadersPath);
        auto path = std::filesystem::current_path();

        std::cout << path;
    	renderData = std::make_shared<Cube>();// new Plane();
        const Material mat(glm::vec4(1, 1, 1, 1));
        renderData->SetMaterial(mat);
        Shader s("defaultShader.vert", "defaultShader.frag");
        renderData->material.setShader(s);

        std::cout << "\n GAME OBJECT CREATED \n";
    }

    bool operator==(const GameObject& other) const {
        return name == other.name;
    }
};