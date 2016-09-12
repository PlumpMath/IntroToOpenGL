#include "MyApplication.h"

RenderGeo::RenderGeo()
{
}

bool RenderGeo::startup()
{

	if (glfwInit() == false)
		return false;

	window = glfwCreateWindow(1280, 720, "Rendering Geometry", nullptr, nullptr);

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

	Gizmos::create(); // initializes

	view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.0f);

	glClearColor(0.0f, .0f, 0.0f, 1);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer

	// create shaders
	const char* vsSource = "#version 410\n \
	layout(location=0) in vec4 position; \
	layout(location=1) in vec4 colour; \
	out vec4 vColour; \
	uniform mat4 projectionViewWorldMatrix; \
	void main() { vColour = colour; gl_Position = projectionViewWorldMatrix * position;}";

	const char* fsSource = "#version 410\n \
	in vec4 vColour; \
	out vec4 fragColor; \
	void main() { fragColor = vColour; }";

	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);

	programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	return true;
}

void RenderGeo::makePlane()
{
	Vertex vertices[4];
	unsigned int indices[4] = { 0,1,2,3 };
	indexCount = 4; // number of indices

	vertices[0].position = vec4(-5, 0, -5, 1);
	vertices[1].position = vec4(5, 0, -5, 1);
	vertices[2].position = vec4(-5, 0, 5, 1);
	vertices[3].position = vec4(5, 0, 5, 1);

	vertices[0].color = vec4(1, 0, 0, 1);
	vertices[1].color = vec4(0, 1, 0, 1);
	vertices[2].color = vec4(0, 0, 1, 1);
	vertices[3].color = vec4(1, 1, 1, 1);

	// Generate our GL buffers
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (4) * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// Generate VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// ......code segment here to bind and fill VBO + IBO

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	delete[] vertices;
}

bool RenderGeo::update()
{
	if (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Gizmos::clear();

		return true;
	}
	return false;
}

void RenderGeo::draw()
{
	Gizmos::draw(projection * view);
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void RenderGeo::shutdown()
{
	Gizmos::destroy();
	glfwDestroyWindow(window);
	glfwTerminate();
}
