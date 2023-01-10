#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;

uniform mat4 MVP;
uniform int FLAG1;

out vec3 fragmentColor;

void main(){	

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);

	// The color of each vertex will be interpolated
	// to produce the color of each fragment
	if(FLAG1 == 1)
		fragmentColor = vec3(0,0,0);
	else
		fragmentColor = vertexColor;
}

