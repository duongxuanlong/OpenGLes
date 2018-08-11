#version 330 core

in vec3 Color;
in vec2 TexCoordinate1;
//in vec2 TexCoordinate2;

uniform sampler2D Texture1;
uniform sampler2D Texture2;

uniform float MixParam;

out vec4 outColor;

float GetValueFromAngle (float angle)
{
	return sin(angle);
}

float GetPixelAtAxis(float x)
{
	float result = -4 * (x * x) + 4 * x;
	return result;
}

void main()
{
	vec4 tex1 = texture(Texture1, TexCoordinate1);
	//vec4 tex2 = texture(Texture2, vec2(1.0 - TexCoordinate1.x, TexCoordinate1.y));
	vec4 tex2 = texture(Texture2, vec2(TexCoordinate1.x, TexCoordinate1.y));
	
	//My own calculation for attrieving the pixels at centers
	float mixtypeX = GetPixelAtAxis(TexCoordinate1.x);
	float mixtypeY = GetPixelAtAxis(TexCoordinate1.y);
	float minMix, maxMix, result;
	if (mixtypeX > mixtypeY)
	{
		minMix = mixtypeY;
		maxMix = mixtypeX;
		result = mixtypeY;
	}
	else
	{
		minMix = mixtypeX;
		maxMix = mixtypeY;
		result = mixtypeX;
	}
	//float result = clamp(1, minMix, maxMix);
	//outColor = tex1 * vec4(Color, 1.0);

	//result = 0.2;
	outColor = mix (tex1, tex2, result);
}