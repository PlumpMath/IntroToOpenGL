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

	view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	projection = glm::perspective(glm::pi<float>() * 0.35f, 16 / 9.f, 0.1f, 1000.0f);
	projectionViewMatrix = projection * view;

	glClearColor(0.25f, .25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer

	
	makeShader();
	makePlane();

	//makeSphere(10, 10, 5);

	return true;
}

bool RenderGeo::update()
{
	if (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
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
	
	int projectionViewUniform = glGetUniformLocation(programID, "projectionViewWorldMatrix");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(projectionViewMatrix));

	glBindVertexArray(VAO);
	glPointSize(5.f);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);

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
	
	//std::string vertString = readFile("vert.vert");
	//const char* vsSource = vertString.c_str();

	//std::string fragString = readFile("frag.frag");
	//const char* fsSource = fragString.c_str();

	const char* vsSource = "#version 410\n \
							layout(location=0) in vec4 Position; \
							layout(location=1) in vec4 Colour; \
							out vec4 vColour; \
							uniform mat4 ProjectionViewWorld; \
							void main() { vColour = Colour; \
							gl_Position = ProjectionViewWorld * Position; }";

	const char* fsSource = "#version 410\n \
							in vec4 vColour; \
							out vec4 FragColor; \
							void main() { FragColor = vColour; }";


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

	vertices[0].colour = vec4(1, 0, 0, 1);
	vertices[1].colour = vec4(0, 1, 0, 1);
	vertices[2].colour = vec4(0, 0, 1, 1);
	vertices[3].colour = vec4(1, 1, 1, 1);

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

Vertex* RenderGeo::makeHalfCircle(const int numPoints, const int radius)
{
	Vertex* halfCircle = new Vertex[numPoints];
	for (int i = 0; i < numPoints; i++)
	{
		float angle = glm::pi<float>() * i / (numPoints - 1);
		halfCircle[i].position = vec4(cos(angle) * radius, sin(angle) * radius, 0, 1);
	}
	return halfCircle;
}

Vertex* RenderGeo::makeSphereVerts(const int numPoints, const int numMeridians, Vertex* vertices)
{
	int counter = 0;
	Vertex* sphereVerts = new Vertex[numPoints*numMeridians];

	for (int i = 0; i < numMeridians; i++)
	{
		phi = 2 * glm::pi<float>() * i / numMeridians;
		for (int j = 0; j < numPoints; j++, counter++)
		{
			newX = vertices[j].position.x;
			newY = vertices[j].position.y * cos(phi) - vertices[j].position.z * sin(phi);
			newZ = vertices[j].position.z * cos(phi) + vertices[j].position.y * sin(phi);

			vertices[counter].position = vec4(newX, newY, newZ, 1);
		}
	}
	return sphereVerts;
}

void RenderGeo::makeSphere(const int numPoints, const int numMeridians, const int radius)
{
	unsigned int totalVerts = numPoints * numMeridians;

	Vertex* vertices = new Vertex[totalVerts];

	vertices = makeHalfCircle(numPoints, radius);
	vertices = makeSphereVerts(numPoints, numMeridians, vertices);

	unsigned int* indices = new unsigned int[totalVerts];
	indexCount = totalVerts;
	for (int i = 0; i < numPoints; i++)
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
	glBufferData(GL_ARRAY_BUFFER, totalVerts * sizeof(Vertex), vertices, GL_STATIC_DRAW);

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

//std::string RenderGeo::readFile(std::string text)
//{
//	std::string line;
//	std::string shader;
//	std::ifstream iFile(text);
//
//	if (iFile.is_open())
//	{
//		while (std::getline(iFile, line))
//		{
//			shader += line + "\n";
//		}
//		iFile.close();
//	}
//	return shader;
//}

std::string loadFile(char fileName[])
{
	std::ifstream f;
	std::string fileData, FullData;
	f.open(fileName, std::ios::in | std::ios::_Nocreate);

	if (f.is_open())
	{
		while (!f.eof())
		{
			std::getline(f, fileData);
			FullData += "\n" + fileData;
		}
	}
	f.close();
	return FullData;
}

