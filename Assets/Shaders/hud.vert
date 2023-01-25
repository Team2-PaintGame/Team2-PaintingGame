#version 400 core

layout(location = 0) in vec3 position;
uniform mat4 modelMatrix = mat4(1.0f);

out vec2 textureCoords;

void main(void) {
	gl_Position = modelMatrix * vec4(position, 1.0);
	textureCoords = vec2((position.x + 1.0) / 2.0, 1 - (position.y + 1.0) / 2.0);
}