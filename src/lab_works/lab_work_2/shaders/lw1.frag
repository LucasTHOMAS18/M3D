#version 450

in vec4 color;
out vec4 aColor;

uniform float uBrightness;

void main() {	
	aColor = vec4(color.rgb * uBrightness, color.a);
}
