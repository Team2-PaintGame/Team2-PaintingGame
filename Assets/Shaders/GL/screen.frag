#version 400 core

in Vertex
{
	vec2 texCoord;
} IN;

out vec4 fragColour;

uniform sampler2D mainTex;

void main(void) {
	fragColour = texture(mainTex, IN.texCoord);
}