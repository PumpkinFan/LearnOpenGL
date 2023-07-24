// GLSL source code for fragment shader
#version 330 core
out vec4 FragColor;
in vec3 ourColor;
//uniform vec4 ourColor; // use a uniform to access values from cpp code 
void main()
{
   FragColor = vec4(ourColor, 1.0);
};
