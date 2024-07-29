#pragma once
#include <string>
#include <cmath>
#include "collision.h"
#include "renderdata.h"
#include <mach-o/dyld.h>

#include<filesystem>


enum ObjectType
{
    type_Platform,
    type_Light,
    type_Player,
    type_Interactable,
    type_MovementPoint
};


//std::wstring convertStringToWString(const std::string& str) {
//    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
//    return converter.from_bytes(str);
//}

// Function to get the path to the current executable
#ifdef __APPLE__
inline std::wstring GetExePath() {
    uint32_t size = 0;
    _NSGetExecutablePath(nullptr, &size); // Get the size of the buffer needed
    std::string buffer(size, '\0'); // Create a buffer of the required size
    _NSGetExecutablePath(buffer.data(), &size); // Get the executable path
    
    // Convert the buffer to a wide string
    std::wstring wbuffer(buffer.begin(), buffer.end());
    
    // Find the last separator and get the directory path
    std::wstring::size_type pos = wbuffer.find_last_of(L"/");
    return wbuffer.substr(0, pos);
//    return convertStringToWString("-excutable file here-");
}
#else
inline std::wstring GetExePath() {
    wchar_t buffer[MAX_PATH];
    etModuleFileName(NULL, buffer, PATH_MAX);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    return std::wstring(buffer).substr(0, pos);
}
#endif


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
        //std::cout << path;
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

        //std::cout << path;
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

        //std::cout << path;
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
        //std::cout << path;
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

        //std::cout << path;
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

        //std::cout << path;
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
        //std::cout << path;
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

        //std::cout << path;
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

        //std::cout << path;
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
        //std::cout << path;
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

        //std::cout << path;
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

        //std::cout << path;
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





class MovementPoint:public GameObject
{
private:

public:
    //array of events
    std::vector<std::shared_ptr<Event>> events;
    void deleteSpecificEvent()
    {
        
    }

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
        //std::cout << path;
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

        //std::cout << path;
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

        //std::cout << path;
        renderData = std::make_shared<Cube>();// new Plane();
        const Material mat(glm::vec4(1, 1, 1, 1));
        renderData->SetMaterial(mat);
        Shader s("defaultShader.vert", "colorShader.frag");
        renderData->material.setShader(s);
        renderData->material.color = glm::vec4(color, 1);
        transform.scale = glm::vec3(0.2);

        //std::cout << "\n GAME OBJECT CREATED \n";
    }

};

class CameraObject:public GameObject
{
    
};




bool ray_collision(glm::vec3 origin, glm::vec3 dir, const std::vector<std::shared_ptr<GameObject>>& gameObjects, std::shared_ptr<GameObject>& obj)
{
    return ray_collision_impl(origin, dir, gameObjects, obj);
}

bool ray_collision(glm::vec3 origin, glm::vec3 dir, const std::vector<std::shared_ptr<MovementPoint>>& movementPoints, std::shared_ptr<MovementPoint>& obj)
{
    return ray_collision_impl(origin, dir, movementPoints, obj);
}

