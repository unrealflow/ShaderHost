#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
// in float Fre;

const vec2 iResolution=vec2(1.0,1.0);
// uniform sampler2D texture_diffuse1;
uniform sampler2D MyTexture;
uniform float iTime;


void main()
{    

//    FragColor=vec4(TexCoords.x,TexCoords.y,0.0,1.0);
    FragColor=texture2D(MyTexture,TexCoords);
}