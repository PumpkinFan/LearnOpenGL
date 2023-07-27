// GLSL source code for vertex shader
#version 330 core
layout (location = 0) in vec3 aPos; // give vertex positions the attribute position 0
layout (location = 1) in vec3 aColor; // gives vertex colors attribute position 1
layout (location = 2) in vec2 aTexCoord; // texture coordinates

// output to the fragment shader
out vec3 ourColor; 
out vec2 texCoord;

uniform float xOffset;
void main()
{
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	texCoord = aTexCoord;

};
