// GLSL source code for fragment shader
#version 330 core
out vec4 FragColor;

// input from vertex shader
in vec3 ourColor; 
in vec2 texCoord; 

uniform vec4 ourColorA; // use a uniform to access values from cpp code 

// why is sampler2D a uniform?
// it corresponds to the `texture unit` which we use to label multiple textures
// the default value is 0 which corresponds to the default active texture 
uniform sampler2D texture0; // creating a GLSL texture sampler object
uniform sampler2D texture1; 

void main()
{
	// using built-in texture function to sample colors from our texture
	//FragColor = texture(ourTexture, texCoord); 

	// `mix` linearly interpolates color based on the final argument 
	// 0.0 -> full texture0, 1.0 -> full texture1
	FragColor = mix(texture(texture0, texCoord), texture(texture1, texCoord), 0.2);
	
	//FragColor = ourColorA;
};
