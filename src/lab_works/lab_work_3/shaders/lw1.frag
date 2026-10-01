#version 450

in vec3 color;
out vec4 aColor;

void main() {	
	aColor = vec4(color.rgb, 1.0);
}
