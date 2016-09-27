#include "MyApplication.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Textures::Textures()
{
}

bool Textures::startup()
{

	if (glfwInit() == false)
		return false;

	window = glfwCreateWindow(1280, 720, "Textures", nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	projection = glm::perspective(glm::pi<float>() * 0.35f, 16 / 9.f, 0.1f, 1000.0f);
	projectionViewMatrix = projection * view;

	glClearColor(0.25f, .25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer

	int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	unsigned char* data;

	// load diffuse map
	data = stbi_load("./rock_diffuse.tga", &imageWidth, &imageHeight, &imageFormat, STBI_default); // read in default texel data for image
	glGenTextures(1, &texture); // generate OpenGL texture handle
	glBindTexture(GL_TEXTURE_2D, texture); // bind the texture to correct dimension slot, 2D in this case
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // specify data for texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // specify filtering so that OpenGL can correctly read the image at various resolutions and ratios for texel : pixel
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // ^
	stbi_image_free(data); // now that a texture is loaded we can free the data loaded with STB

	// load normal map
	data = stbi_load("./rock_normal.tga", &imageWidth, &imageHeight, &imageFormat, STBI_default);
	glGenTextures(1, &normal);
	glBindTexture(GL_TEXTURE_2D, normal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_image_free(data);

	const char* vsSource = "#version 410\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec2 TexCoord; \
							layout(location=2) in vec4 Normal; \
							layout(location=3) in vec4 Tangent; \
							out vec2 vTexCoord; \
							out vec3 vNormal; \
							out vec3 vTangent; \
							out vec3 vBiTangent; \
							uniform mat4 ProjectionView; \
							void main() { vTexCoord = TexCoord; \
							vNormal = Normal.xyz; \
							vTangent = Tangent.xyz; \
							vBiTangent = cross(vNormal, vTangent); \
							gl_Position = ProjectionView * Position; }";

	const char* fsSource = "#version 410\n \
							in vec2 vTexCoord; \
							in vec3 vNormal; \
							in vec3 vTangent; \
							in vec3 vBiTangent; \
							out vec4 FragColor; \
							uniform vec3 LightDir; \
							uniform sampler2D diffuse; \
							uniform sampler2D normal; \
							void main() { \
							mat3 TBN = mat3( \
							normalize( vTangent ), \
							normalize( vBiTangent ), \
							normalize( vNormal )); \
							vec3 N = texture(normal, \
							vTexCoord).xyz * 2 - 1; \
							float d = max( 0, dot( \
							normalize( TBN * N ), \
							normalize( LightDir ))); \
							FragColor = texture(diffuse, vTexCoord); \
							FragColor.rgb = FragColor.rgb * d;}";

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);
	programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// 2-triangle quad with texture coordinates
	AdvancedVertex vertexData[] = 
	{
		{-5, 0, 5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1 },
		{ 5, 0, 5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1 },
		{ 5, 0,-5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0 },
		{-5, 0,-5, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0 },
	};
	unsigned int indexData[] = 
	{
		0, 1, 2,
		0, 2, 3,
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(AdvancedVertex) * 4, vertexData, GL_STATIC_DRAW);
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(AdvancedVertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(AdvancedVertex), ((char*)0) + 48);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(AdvancedVertex), ((char*)0) + 16);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(AdvancedVertex), ((char*)0) + 32);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}

bool Textures::update()
{
	if (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		return true;
	}
	return false;
}

void Textures::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use our texture program
	glUseProgram(programID);

	// bind the camera
	int projectionViewUniform = glGetUniformLocation(programID, "ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, GL_FALSE, glm::value_ptr(projectionViewMatrix));

	// set texture slot
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normal);

	// tell the shader where it is
	projectionViewUniform = glGetUniformLocation(programID, "diffuse");
	glUniform1i(projectionViewUniform, 0);
	projectionViewUniform = glGetUniformLocation(programID, "normal");
	glUniform1i(projectionViewUniform, 1);

	// bind the light
	vec3 light(sin(glfwGetTime()), 1, cos(glfwGetTime()));
	projectionViewUniform = glGetUniformLocation(programID, "LightDir");
	glUniform3f(projectionViewUniform, light.x, light.y, light.z);

	// draw
	glBindVertexArray(VAO); 
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Textures::shutdown()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}