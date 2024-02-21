#pragma once
#include <string>
#include<cmath>
#include "collision.h"
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


class Sphere:public ObjectRenderData
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









#include<filesystem>
// Function to get the path to the current executable
inline std::wstring GetExePath() {
    wchar_t buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    return std::wstring(buffer).substr(0, pos);
}

enum ObjectType
{
	type_Platform,
    type_Light,
    type_Player,
    type_Interactable,
    type_MovementPoint
};

class GameObject
{
public:
    ObjectType objectType;
    std::shared_ptr<ObjectRenderData> renderData;//stores
    std::string name;//name of the game object
    ObjectTransform transform;//transform data of the gameobject
    BoxCollider collider;
    GameObject() {
        // Set default values or perform initialization if needed
        name = "nothing selected";
    }

	GameObject(const std::string name) : name(name)//test constructor
	{
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

    GameObject(const std::string name, ObjectTransform transformData) :name(name), transform(transformData)
    {
        std::wstring exePath = GetExePath();
        std::filesystem::path shadersPath = std::filesystem::path(exePath) / "Shaders"; // Assuming Shaders folder is in the same directory as the executable

        // Set the path to the shaders
        std::filesystem::current_path(shadersPath);
        auto path = std::filesystem::current_path();

        std::cout << path;
    	renderData = std::make_shared<Cube>();// new Plane();
        const Material mat(glm::vec4(1, 1, 1, 1));
        renderData->SetMaterial(mat);
        Shader s("defaultShader.vert", "colorShader.frag");
        renderData->material.setShader(s);

        std::cout << "\n GAME OBJECT CREATED \n";
    }
    GameObject(const std::string name, ObjectTransform transformData, glm::vec3 color) :name(name), transform(transformData)
    {
        std::wstring exePath = GetExePath();
        std::filesystem::path shadersPath = std::filesystem::path(exePath) / "Shaders"; // Assuming Shaders folder is in the same directory as the executable

        // Set the path to the shaders
        std::filesystem::current_path(shadersPath);
        auto path = std::filesystem::current_path();

        std::cout << path;
        renderData = std::make_shared<Cube>();// new Plane();
        const Material mat(glm::vec4(1, 1, 1, 1));
        renderData->SetMaterial(mat);
        Shader s("defaultShader.vert", "colorShader.frag");
        renderData->material.setShader(s);
        renderData->material.color = glm::vec4(color,1);
        std::cout << "\n GAME OBJECT CREATED \n";
    }
    virtual ~GameObject() = default;  // Virtual destructor
    bool operator==(const GameObject& other) const {
        return name == other.name;
    }
};


class Platform:public GameObject
{
public:
    Platform() {
        // Set default values or perform initialization if needed
        name = "nothing selected";
    }

    Platform(const std::string name)
    {
        GameObject::name = name;
        objectType = ObjectType::type_Platform;
        std::wstring exePath = GetExePath();
        std::filesystem::path shadersPath = std::filesystem::path(exePath) / "Shaders"; // Assuming Shaders folder is in the same directory as the executable
        // Set the path to the shaders
        std::filesystem::current_path(shadersPath);
        auto path = std::filesystem::current_path();
        std::cout << path;
        renderData = std::make_shared<Cube>();// new Plane();
        const Material mat(glm::vec4(1, 1, 1, 1));
        renderData->SetMaterial(mat);
        Shader s("defaultShader.vert", "defaultShaderwithlighting.frag");
        renderData->material.setShader(s);
        std::cout << "\n GAME OBJECT CREATED \n";
    }

    Platform(const std::string name, ObjectTransform transformData) 
    {
        objectType = ObjectType::type_Platform;

        GameObject::name = name;
        transform = transformData;

        std::wstring exePath = GetExePath();
        std::filesystem::path shadersPath = std::filesystem::path(exePath) / "Shaders"; // Assuming Shaders folder is in the same directory as the executable

        // Set the path to the shaders
        std::filesystem::current_path(shadersPath);
        auto path = std::filesystem::current_path();

        std::cout << path;
        renderData = std::make_shared<Cube>();// new Plane();
        const Material mat(glm::vec4(1, 1, 1, 1));
        renderData->SetMaterial(mat);
        Shader s("defaultShader.vert", "defaultShaderwithlighting.frag");
        renderData->material.setShader(s);

        std::cout << "\n GAME OBJECT CREATED \n";
    }
    Platform(const std::string name, ObjectTransform transformData, glm::vec3 color) 
    {

        GameObject::name = name;
        transform = transformData;
        objectType = ObjectType::type_Platform;

        std::wstring exePath = GetExePath();
        std::filesystem::path shadersPath = std::filesystem::path(exePath) / "Shaders"; // Assuming Shaders folder is in the same directory as the executable

        // Set the path to the shaders
        std::filesystem::current_path(shadersPath);
        auto path = std::filesystem::current_path();

        std::cout << path;
        renderData = std::make_shared<Cube>();// new Plane();
        const Material mat(glm::vec4(1, 1, 1, 1));
        renderData->SetMaterial(mat);
        Shader s("defaultShader.vert", "defaultShaderwithlighting.frag");
        renderData->material.setShader(s);
        renderData->material.color = glm::vec4(color, 1);
        std::cout << "\n GAME OBJECT CREATED \n";
    }

    
};

class Light:public GameObject
{
private:
    float intensity=0.5;

public:
    Light() {
        // Set default values or perform initialization if needed
        name = "nothing selected";
    }

    Light(const std::string name)
    {
        GameObject::name = name;
        objectType = ObjectType::type_Light;

        std::wstring exePath = GetExePath();
        std::filesystem::path shadersPath = std::filesystem::path(exePath) / "Shaders"; // Assuming Shaders folder is in the same directory as the executable
        // Set the path to the shaders
        std::filesystem::current_path(shadersPath);
        auto path = std::filesystem::current_path();
        std::cout << path;
        renderData = std::make_shared<Cube>();// new Plane();
        const Material mat(glm::vec4(1, 1, 1, 1));
        renderData->SetMaterial(mat);
        Shader s("defaultShader.vert", "lightfragmentshader.frag");
        renderData->material.setShader(s);
        std::cout << "\n GAME OBJECT CREATED \n";
    }

    Light(const std::string name, ObjectTransform transformData) 
    {
        GameObject::name = name;
        objectType = ObjectType::type_Light;

        GameObject::transform = transformData;
        std::wstring exePath = GetExePath();
        std::filesystem::path shadersPath = std::filesystem::path(exePath) / "Shaders"; // Assuming Shaders folder is in the same directory as the executable

        // Set the path to the shaders
        std::filesystem::current_path(shadersPath);
        auto path = std::filesystem::current_path();

        std::cout << path;
        renderData = std::make_shared<Cube>();// new Plane();
        const Material mat(glm::vec4(1, 1, 1, 1));
        renderData->SetMaterial(mat);
        Shader s("defaultShader.vert", "lightfragmentshader.frag");
        renderData->material.setShader(s);

        std::cout << "\n GAME OBJECT CREATED \n";
    }
    Light(const std::string name, ObjectTransform transformData, glm::vec3 color)
	{
        GameObject::name = name;
        GameObject::transform = transformData;
        objectType = ObjectType::type_Light;

        std::wstring exePath = GetExePath();
        std::filesystem::path shadersPath = std::filesystem::path(exePath) / "Shaders"; // Assuming Shaders folder is in the same directory as the executable

        // Set the path to the shaders
        std::filesystem::current_path(shadersPath);
        auto path = std::filesystem::current_path();

        std::cout << path;
        renderData = std::make_shared<Cube>();// new Plane();
        const Material mat(glm::vec4(1, 1, 1, 1));
        renderData->SetMaterial(mat);
        Shader s("defaultShader.vert", "lightfragmentshader.frag");
        renderData->material.setShader(s);
        renderData->material.color = glm::vec4(color, 1);
        std::cout << "\n GAME OBJECT CREATED \n";
    }

	void setIntensity(float newIntensity)
    {
        intensity = newIntensity;
    }

	float getIntensity() const { return intensity; }
};

class Player: public GameObject
{
private:

public:
    Player() {
        // Set default values or perform initialization if needed
        name = "nothing selected";
    }

    Player(const std::string name)
    {
        GameObject::name = name;
        objectType = ObjectType::type_Player;

        std::wstring exePath = GetExePath();
        std::filesystem::path shadersPath = std::filesystem::path(exePath) / "Shaders"; // Assuming Shaders folder is in the same directory as the executable
        // Set the path to the shaders
        std::filesystem::current_path(shadersPath);
        auto path = std::filesystem::current_path();
        std::cout << path;
        renderData = std::make_shared<Cube>();// new Plane();
        const Material mat(glm::vec4(1, 1, 1, 1));
        renderData->SetMaterial(mat);
        Shader s("defaultShader.vert", "defaultShaderwithlighting.frag");
        renderData->material.setShader(s);
        std::cout << "\n GAME OBJECT CREATED \n";
    }

    Player(const std::string name, ObjectTransform transformData)
    {
        GameObject::name = name;
        objectType = ObjectType::type_Player;

        GameObject::transform = transformData;
        std::wstring exePath = GetExePath();
        std::filesystem::path shadersPath = std::filesystem::path(exePath) / "Shaders"; // Assuming Shaders folder is in the same directory as the executable

        // Set the path to the shaders
        std::filesystem::current_path(shadersPath);
        auto path = std::filesystem::current_path();

        std::cout << path;
        renderData = std::make_shared<Cube>();// new Plane();
        const Material mat(glm::vec4(1, 1, 1, 1));
        renderData->SetMaterial(mat);
        Shader s("defaultShader.vert", "defaultShaderwithlighting.frag");
        renderData->material.setShader(s);

        std::cout << "\n GAME OBJECT CREATED \n";
    }
    Player(const std::string name, ObjectTransform transformData, glm::vec3 color)
    {
        GameObject::name = name;
        GameObject::transform = transformData;
        objectType = ObjectType::type_Player;

        std::wstring exePath = GetExePath();
        std::filesystem::path shadersPath = std::filesystem::path(exePath) / "Shaders"; // Assuming Shaders folder is in the same directory as the executable

        // Set the path to the shaders
        std::filesystem::current_path(shadersPath);
        auto path = std::filesystem::current_path();

        std::cout << path;
        renderData = std::make_shared<Cube>();// new Plane();
        const Material mat(glm::vec4(1, 1, 1, 1));
        renderData->SetMaterial(mat);
        Shader s("defaultShader.vert", "defaultShaderwithlighting.frag");
        renderData->material.setShader(s);
        renderData->material.color = glm::vec4(color, 1);
        std::cout << "\n GAME OBJECT CREATED \n";
    }
    
    void changePosition(glm::vec3 newPos)
    {
        transform.translation = newPos;
        transform.CalculateModel();
    }
};


enum EventTime
{
	Enter,
    Exit
};
enum EventType
{
	TextBox,
    Print,
    Inventory,
    InventoryConditional
};

struct Item
{
public:
    std::string name;
    int count = 1;
};

class ConditionalEventData
{
private:
    Item item;
public:
    ConditionalEventData()
    {
        item.name = "";
        item.count = 0;
    }
    ConditionalEventData(std::string name, int count)
    {
        setItem(name, count);
    }
    void setItem(std::string itemName, int n)
    {
        item.name = itemName;
        item.count = n;
    }
    bool CheckConditionSatisfied()
    {
	    
    }
    void setCount(int n) { item.count = n; }
    Item getItem() { return item; }
};

class Inventory
{
private:
    std::vector<std::string> itemNames;
    std::vector<Item> itemsHeldCurrently;
public:
    std::vector<std::string>& getItemNames() { return itemNames; }
    std::vector<Item>& getCurrentInventory() { return itemsHeldCurrently; }
    void clearData()
    {
        itemsHeldCurrently.clear();
        itemNames.clear();
    }
    void clearPlayerInventory()
    {
        itemsHeldCurrently.clear();
    }
    void addItemToList(const std::string i)
    {
        itemNames.push_back(i);
    }
    void removeLastItemFromList()
    {
        if (!itemNames.empty())
        {
            itemNames.pop_back();
        }
    }
    void addItemToInventory(Item i)
    {

        for (auto& item : itemsHeldCurrently)
        {
            if (item.name == i.name)
            {
                item.count += i.count;

                if (item.count < 0)
                {
                    item.count = 0;
                }
                return;
            }
        }
        itemsHeldCurrently.push_back(i);
    }
    void removeItemFromInventory(Item i)
    {
        for (auto it = itemsHeldCurrently.begin(); it != itemsHeldCurrently.end(); ++it)
        {
            if (it->name == i.name)
            {
                it->count -= i.count;
                if (it->count <= 0)
                {
                    itemsHeldCurrently.erase(it);
                }
                break;
            }
        }
    }
    bool checkForItem(std::string itemname, int count)
    {
        for (auto& item : itemsHeldCurrently)
        {
            if (item.name == itemname && item.count >= count)
            {
                return true;
            }
        }
        return false;
    }

} manager_Inventory;


class Event
{
private:
    EventTime eventTime=Enter;
    std::string eventName;
    EventType eventType;

    //allow user to create conditions if they want
    bool isConditional=false;

public:
    ConditionalEventData conditionalEventData;

    Event() {}
    Event(std::string ename, EventType etype, EventTime etime)
    {
        setEventType(etype);
        setEventName(ename);
        setEventTime(etime);
    }
    void setEventTime(EventTime type)
    {
        eventTime = type;
    }
    void setEventName(std::string name)
    {
        eventName = name;
    }
    void setEventType(EventType type)
    {
        eventType = type;
    }
    virtual void doThing(){}
    void runEvent()
    {
        if (isConditional)
        {
            conditionalDoThing();
        }
        else
            doThing();
    }
	void conditionalDoThing()
    {
	    if(manager_Inventory.checkForItem(conditionalEventData.getItem().name,conditionalEventData.getItem().count))
	    {
            doThing();
	    }
    }
    std::string getName() { return eventName; }
    EventTime getTime() { return eventTime; }
    EventType getType() { return eventType; }
    void setConditional(bool con) { isConditional = con; }
};


class MovementPoint:public GameObject
{
private:

public:
    //array of events
    std::vector<std::shared_ptr<Event>> events;

   

    MovementPoint() {
        // Set default values or perform initialization if needed
        name = "nothing selected";
    }

    MovementPoint(const std::string name)
    {
        GameObject::name = name;
        objectType = ObjectType::type_MovementPoint;

        std::wstring exePath = GetExePath();
        std::filesystem::path shadersPath = std::filesystem::path(exePath) / "Shaders"; // Assuming Shaders folder is in the same directory as the executable
        // Set the path to the shaders
        std::filesystem::current_path(shadersPath);
        auto path = std::filesystem::current_path();
        std::cout << path;
        renderData = std::make_shared<Cube>();// new Plane();


        const Material mat(glm::vec4(1, 1, 1, 1));
        renderData->SetMaterial(mat);
        Shader s("defaultShader.vert", "colorShader.frag");
        renderData->material.setShader(s);
        transform.scale = glm::vec3(0.2);

    	std::cout << "\n GAME OBJECT CREATED \n";

    }

    MovementPoint(const std::string name, ObjectTransform transformData)
    {
        GameObject::name = name;
        objectType = ObjectType::type_MovementPoint;

        GameObject::transform = transformData;
        std::wstring exePath = GetExePath();
        std::filesystem::path shadersPath = std::filesystem::path(exePath) / "Shaders"; // Assuming Shaders folder is in the same directory as the executable

        // Set the path to the shaders
        std::filesystem::current_path(shadersPath);
        auto path = std::filesystem::current_path();

        std::cout << path;
        renderData = std::make_shared<Cube>();// new Plane();
        const Material mat(glm::vec4(1, 1, 1, 1));
        renderData->SetMaterial(mat);
        Shader s("defaultShader.vert", "colorShader.frag");
        renderData->material.setShader(s);
        transform.scale = glm::vec3(0.2);

        std::cout << "\n GAME OBJECT CREATED \n";
    }
    MovementPoint(const std::string name, ObjectTransform transformData, glm::vec3 color)
    {
        GameObject::name = name;
        GameObject::transform = transformData;
        objectType = ObjectType::type_MovementPoint;

        std::wstring exePath = GetExePath();
        std::filesystem::path shadersPath = std::filesystem::path(exePath) / "Shaders"; // Assuming Shaders folder is in the same directory as the executable

        // Set the path to the shaders
        std::filesystem::current_path(shadersPath);
        auto path = std::filesystem::current_path();

        std::cout << path;
        renderData = std::make_shared<Cube>();// new Plane();
        const Material mat(glm::vec4(1, 1, 1, 1));
        renderData->SetMaterial(mat);
        Shader s("defaultShader.vert", "colorShader.frag");
        renderData->material.setShader(s);
        renderData->material.color = glm::vec4(color, 1);
        transform.scale = glm::vec3(0.2);

    	std::cout << "\n GAME OBJECT CREATED \n";
    }

};

class CameraObject:public GameObject
{
	
};




bool ray_collision(glm::vec3 origin, glm::vec3 dir, std::vector<std::shared_ptr<GameObject>> gameObjects, std::shared_ptr<GameObject>& obj)
{
    float totaldist = 0;
	glm::vec3 current_pos = origin;
    for (int i = 0; i < MAX_RAY_ITERATIONS; i++)
    {
        float min_dist = std::numeric_limits<float>::infinity();
        double calc_distance = 0;
        //find if collision
        //search shortest dist
        for (std::shared_ptr<GameObject>& gameObject : gameObjects)
        {
        	if (gameObject->collider.CollisionTest(current_pos,gameObject->transform.translation, gameObject->transform.rotation
                , gameObject->transform.scale))
            {
                //std::cout <<std::endl <<"COLLISION";
                //std::cout <<std::endl<< gameObject->name;
                obj= gameObject;
                return true;
            }
           // calc_distance = glm::distance(gameObject->transform.translation, current_pos);
            calc_distance = gameObject->collider.sdBox(current_pos, gameObject->transform.translation, gameObject->transform.rotation
                , gameObject->transform.scale);
            //std::cout << std::endl << gameObject->name << " - distance: " << calc_distance << " ";
            if (calc_distance < min_dist)
            {

                min_dist = calc_distance;
            }
        }
        //travel that dist
        totaldist += min_dist;
        //if (min_dist - 0.1 < 0.0001)std::cout << "\nSMALL\n ";
        //std::cout << "total dist: " << totaldist;
        current_pos = origin+dir * totaldist;
        //std::cout<<"\nCurrent pos: "<<current_pos.x<<" "<<current_pos.y<<" "<<current_pos.z;
        //if we go beyond, stop
        if (totaldist > 100000)
        {
	        //std::cout << "reached infinity";
        	return false;
        }
    }
    return false;
}



bool ray_collision(glm::vec3 origin, glm::vec3 dir, std::vector<std::shared_ptr<MovementPoint>> movementPoints, std::shared_ptr<MovementPoint>& obj)
{
    float totaldist = 0;
    glm::vec3 current_pos = origin;
    for (int i = 0; i < MAX_RAY_ITERATIONS; i++)
    {
        float min_dist = std::numeric_limits<float>::infinity();
        double calc_distance = 0;
        //find if collision
        //search shortest dist
        for (std::shared_ptr<MovementPoint>& point : movementPoints)
        {
            if (point->collider.CollisionTest(current_pos, point->transform.translation, point->transform.rotation
                , point->transform.scale))
            {
                //std::cout <<std::endl <<"COLLISION";
                //std::cout <<std::endl<< gameObject->name;
                obj = point;
                return true;
            }
            // calc_distance = glm::distance(gameObject->transform.translation, current_pos);
            calc_distance = point->collider.sdBox(current_pos, point->transform.translation, point->transform.rotation
                , point->transform.scale);
            //std::cout << std::endl << gameObject->name << " - distance: " << calc_distance << " ";
            if (calc_distance < min_dist)
            {

                min_dist = calc_distance;
            }
        }
        //travel that dist
        totaldist += min_dist;
        //if (min_dist - 0.1 < 0.0001)std::cout << "\nSMALL\n ";
        //std::cout << "total dist: " << totaldist;
        current_pos = origin + dir * totaldist;
        //std::cout<<"\nCurrent pos: "<<current_pos.x<<" "<<current_pos.y<<" "<<current_pos.z;
        //if we go beyond, stop
        if (totaldist > 100000)
        {
            //std::cout << "reached infinity";
            return false;
        }
    }
    return false;
}