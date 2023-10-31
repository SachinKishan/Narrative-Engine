#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{          

vec4 texColor = texture(texture1, TexCoords);
   // if(texColor.a < 0.1)
     //   discard;
    //float average = 0.2126 * texColor.r + 0.7152 * texColor.g + 0.0722 * texColor.b;
    //FragColor = vec4(average, average, average, 1.0);
    FragColor = texColor;
}  