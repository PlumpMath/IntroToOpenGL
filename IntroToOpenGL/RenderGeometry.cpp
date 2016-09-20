﻿#include "MyApplication.h"

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

	view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	projection = glm::perspective(glm::pi<float>() * 0.35f, 16 / 9.f, 0.1f, 1000.0f);
	projectionViewMatrix = projection * view;

	glClearColor(0.25f, .25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer

	//makePlane();
	makeShader();
	makeSphere(10, 10, 5);

	return true;
}

bool RenderGeo::update()
{
	if (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		return true;
	}
	return false;
}

void RenderGeo::draw()
{
	/*GL_COLOR_BUFFER_BIT informs OpenGL to wipe the back - buffer colours clean.
	GL_DEPTH_BUFFER_BIT informs it to clear the distance to the closest pixels.If we didn�t do this then
	OpenGL may think the image of the last frame is still there and our new visuals may not display.*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programID);
	unsigned int projectionViewUniform = glGetUniformLocation(programID, "projectionViewWorldMatrix");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(projectionViewMatrix));

	glBindVertexArray(VAO);
	glPointSize(5.f);
	glDrawElements(GL_POINTS, indexCount, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void RenderGeo::shutdown()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void RenderGeo::makeShader()
{
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
}

void RenderGeo::makePlane()
{
	Vertex vertices[4];
	unsigned int indices[4] = { 0, 1, 2, 3 };
	indexCount = 4; // number of indices

	vertices[0].position = vec4(-5, 0, -5, 1);
	vertices[1].position = vec4(5, 0, -5, 1);
	vertices[2].position = vec4(-5, 0, 5, 1);
	vertices[3].position = vec4(5, 0, 5, 1);

	vertices[0].color = vec4(1, 0, 0, 1);
	vertices[1].color = vec4(0, 1, 0, 1);
	vertices[2].color = vec4(0, 0, 1, 1);
	vertices[3].color = vec4(1, 1, 1, 1);

	// Generate our GL buffers, making our handles
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	// Generate VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//bind the vertex bufffer handle
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//put the vertex info on the card
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	//bind the index buffer handle 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//put the index info on the card
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

std::vector<vec4> RenderGeo::makeHalfCircle(int numPoints, float radius)
{
	std::vector<vec4> halfCircle;
	for (int i = 0; i < numPoints; i++)
	{
		angle = glm::pi<float>() * i / (numPoints - 1);
		vec4 pos = vec4(cos(angle) * radius, sin(angle) * radius, 0, 1);
		halfCircle.push_back(pos);
	}
	return halfCircle;
}


void RenderGeo::makeSphere(int numPoints, int numMeridians, float radius)
{
	int counter = numPoints;
	pi = glm::pi<float>();
	std::vector<vec4> vertices = makeHalfCircle(numPoints, radius);

	for (int i = 1; i < numMeridians; i++)
	{ 
		phi = 2 * pi * i / numMeridians;
		for (int j = 0; j < numPoints; j++, counter++)
		{
			newX = vertices[j].x;
			newY = vertices[j].y * cos(phi) - vertices[j].z * sin(phi);
			newZ = vertices[j].z * cos(phi) + vertices[j].y * sin(phi);

			vertices.push_back(vec4(newX, newY, newZ, 1));
		}
	}

	unsigned int* indices = new unsigned int[vertices.size()];
	indexCount = vertices.size();

	for (int i = 0; i < vertices.size(); i++)
	{
		indices[i] = i;
	}


	//generate our GL buffers, making our handles
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	//generate VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//bind the vertex buffer handle
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//put the vertex info on the card
	glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	//bind the index buffer handle 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//put the index info on the card
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	//position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	//color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	//safety
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
