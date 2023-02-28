#version 400 core

out vec4 fragColour;

uniform sampler2D mainTex;

void main(void) {
	fragColour = texture(mainTex, textureCoords);
}