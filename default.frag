#version 460 core

out vec4 FragColor;

uniform vec4 triangleColor;

void main() {
   FragColor = triangleColor;
}