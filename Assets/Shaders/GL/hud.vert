#version 400 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texCoord;

uniform mat4 modelMatrix = mat4(1.0f);

out vec2 textureCoords;

void main(void) {
	gl_Position = modelMatrix * vec4(position, 1.0);
	textureCoords = texCoord; 
}