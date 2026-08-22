#version 330

out vec4 FragColor;

in vec3 vertexColor;

uniform vec4 vCubeColor = vec4(1.0, 1.0, 1.0, 1.0);
uniform bool bCubeColor;

void main()
{
	if (bCubeColor)
		FragColor = vCubeColor;
	else
		FragColor = vec4(vertexColor, 1.0);	
}