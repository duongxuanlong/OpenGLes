#version 330 core

in vec3 aPos;
in vec3 aColor;
in vec2 aTexCoordinate;

out vec3 Color;
out vec2 TexCoordinate;

uniform mat4 TranFormMatrix;

void main()
{
	gl_Position = TranFormMatrix * vec4(aPos, 1.0f);
	Color = aColor;
	TexCoordinate = aTexCoordinate;
}