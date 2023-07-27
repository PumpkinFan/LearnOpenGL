// GLSL source code for fragment shader
#version 330 core
out vec4 FragColor;

// input from vertex shader
in vec3 ourColor; 
in vec2 texCoord; 

uniform vec4 ourColorA; // use a uniform to access values from cpp code 
uniform sampler2D ourTexture; // creating a GLSL texture sampler object

void main()
{
	// using built-in texture function to sample colors from our texture
	//FragColor = texture(ourTexture, texCoord); 

	// get a little funky by mixing texture with color data
	FragColor = texture(ourTexture, texCoord) * vec4(ourColor, 1.0);
	
	//FragColor = ourColorA;
};
