#version 330 core

layout(location=0) out vec4 vFragColor;	//fragment shader output
smooth in vec2 vUV;
uniform sampler2D textureMap;

void main()
{
	//output a constant white colour vec4(1,1,1,1)
	vFragColor = texture(textureMap, vUV);
}