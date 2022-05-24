#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 InPos;
layout(location = 1) in vec3 InColor;

layout(location = 0) out vec3 FragColor;

void main() 
{
	gl_Position = vec4(InPos, 1.0);

	FragColor = InColor;
}