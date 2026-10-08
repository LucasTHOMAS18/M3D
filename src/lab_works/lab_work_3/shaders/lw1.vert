#version 450

layout( location = 0 ) in vec3 aVertexPosition;
layout( location = 1 ) in vec3 aVertexColor;

uniform mat4 uTransform;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

out vec3 color;
void main() {	
	gl_Position = uProjectionMatrix * uViewMatrix * uTransform * vec4(aVertexPosition, 1.0);
	color = aVertexColor;
}
