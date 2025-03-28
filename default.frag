#version 460 core

out vec4 FragColor;

in vec2 TexCoord;

uniform vec4 triangleColor;
uniform sampler2D ourTexture;

void main() {
	//FragColor = triangleColor;
	FragColor = texture(ourTexture, TexCoord);
}