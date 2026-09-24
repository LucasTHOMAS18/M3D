#version 450

layout( location = 0 ) in vec2 aVertexPosition;
layout( location = 1 ) in vec4 aVertexColor;

out vec4 color;
uniform float uTranslationX;

void main() {	
	gl_Position = vec4(aVertexPosition + vec2(uTranslationX, 0), 0.0, 1.0);
	color = aVertexColor;
}
