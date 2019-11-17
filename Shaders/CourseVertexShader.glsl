#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec4 vertexCol;

out Vertex {
	vec4 colour;
} OUT;

void main (void) {
	gl_Position = vec4(vertexPos, 1.0);
	OUT.colour = vertexCol;
}
