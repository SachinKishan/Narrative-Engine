#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;
layout (location = 1) in vec2 aTexCoords;

out vec3 normal;
out vec3 FragPos;  
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;


void main()
{

    TexCoords=aTexCoords;
    //normal = mat3(transpose(inverse(model))) * aNormal;  
    FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection*view*model*vec4(aPos,1.0f);
     
}