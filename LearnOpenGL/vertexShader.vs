// GLSL source code for vertex shader
// very simple shader - just outputs the vertices with no transformation
#version 330 core
layout (location = 0) in vec3 aPos; // give vertex positions the attribute position 0
layout (location = 1) in vec3 aColor; // gives vertex colors attribute position 1
out vec3 ourColor; // output ourColor to the fragment shader
void main()
{
    gl_Position = vec4(aPos, 1.0); // we can directly plug a vec3 (aPos here) into a vec4 constructor
	ourColor = aColor;
};
