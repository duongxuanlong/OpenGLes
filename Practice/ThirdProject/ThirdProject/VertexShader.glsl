#version 330 core

in vec3 aPos;
in vec3 aColor;
in vec2 aTexCoordinate1;
//in vec2 aTexCoordinate2;

out vec3 Color;
out vec2 TexCoordinate1;
//out vec2 TexCoordinate2;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	
	Color = aColor;
	TexCoordinate1 = vec2(aTexCoordinate1.x, aTexCoordinate1.y);
	//TexCoordinate2 = aTexCoordinate2;
}