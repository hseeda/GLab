#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;

uniform mat4 MVP;
uniform int FLAG1;

out vec2 imageCoords;

void main(){	

	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);


    if(FLAG1 == 0)
    imageCoords = vec2( vertexColor[0], vertexColor[1]); // for matlab.png
    else
    imageCoords = vec2(00.0,0.0); // for matlab.png
}

