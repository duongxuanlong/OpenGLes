#version 330 core

in vec3 Color;
in vec2 TexCoordinate;

uniform sampler2D Texture1;
uniform sampler2D Texture2;

out vec4 OutColor;

void main()
{
	vec4 texColor1 = texture(Texture1, TexCoordinate);
	vec4 texColor2 = texture(Texture2, TexCoordinate);

	OutColor = mix(texColor1, texColor2, 0.2f);
}