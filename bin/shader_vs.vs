#version 330 core
layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aNormal;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec2 iResolution;
// out float Fre;
uniform vec2 iResolution0;
// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;
// uniform vec3 c_Pos;
void main()
{
    TexCoords = vec2(aTexCoords.x,1.0-aTexCoords.y)*iResolution0;    
    iResolution=iResolution0;
    vec4 pos=vec4(aPos, 1.0);

    gl_Position=pos;
}