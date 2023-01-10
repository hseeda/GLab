#version 330 core

// Interpolated values from the vertex shaders
in vec3 fragmentColor;
in vec2 UV;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform float FLAG1;

// Ouput data
out vec3 color;

void main(){

	// Output color = color specified in the vertex shader, 
	// interpolated between all 3 surrounding vertices
	// color = fragmentColor;

	// Output color = color of the texture at the specified UV
	color = mix(texture( myTextureSampler, UV ).rgb,fragmentColor, FLAG1);

}