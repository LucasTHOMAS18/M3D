#version 450

layout( location = 0 ) in vec3 aVertexPosition;
layout( location = 1 ) in vec3 aVertexColor;

uniform mat4 uTransform;

out vec3 color;
void main() {	
	gl_Position = vec4(aVertexPosition, 1.0) * uTransform;
	color = aVertexColor;
}
