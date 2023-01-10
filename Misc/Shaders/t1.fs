#version 330 core

in vec2 imageCoords;

uniform sampler2D IMAGE;

out vec4 color;

void main()
{
	vec3 col = texture(IMAGE, imageCoords).rgb;
    color = vec4(col, 1.0);
}