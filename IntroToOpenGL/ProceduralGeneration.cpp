#include "MyApplication.h"
#include "stb_image.h"

ProcGen::ProcGen()
{
}

bool ProcGen::startup()
{

	if (glfwInit() == false)
		return false;

	window = glfwCreateWindow(1280, 720, "Procedural Generation", nullptr, nullptr);

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

	int dims = 64;
	float *perlinData = new float[dims * dims];
	float scale = (1.0f / dims) * 3;
	int octaves = 6;

	for (int x = 0; x < 64; ++x)
	{
		for (int y = 0; y < 64; ++y)
		{
			float amplitude = 1.f;
			float persistence = 0.3f;
			perlinData[y* dims + x] = 0;

			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(2, (float)o);
				float perlin_sample = glm::perlin(vec2((float)x, (float)y) * scale * freq) * 0.5f + 0.5f;
				perlinData[y * dims + x] += perlin_sample * amplitude;
				amplitude *= persistence;
			}
		}
	}

	glGenTextures(1, &perlinTexture);
	glBindTexture(GL_TEXTURE_2D, perlinTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 64, 64, 0, GL_RED, GL_FLOAT, perlinData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	const char* vsSource = "#version 410 \
							layout(location=0) in vec4 position; \
							layout(location=1) in vec2 texcoord; \
							out vec2 frag_texcoord; \
							uniform mat4 ProjectionView; \
							uniform sampler2D perlin_texture; \
							void main() { \
							vec4 pos = position; \
							pos.y += texture(perlin_texture, texcoord).r * 5; \
							frag_texcoord = texcoord; \
							gl_Position = ProjectionView * pos; }";

	const char* fsSource = "#version 410 \
							in vec2 frag_texcoord; \
							out vec4 FragColor; \
							uniform sampler2D perlin_texture; \
							void main() { \
							FragColor = texture(perlin_texture, frag_texcoord).rrrr; \
							FragColor.a = 1;}";

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

bool ProcGen::update()
{
	if (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		return true;
	}
	return false;
}

void ProcGen::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use our texture program
	glUseProgram(programID);

	// bind the camera
	int projectionViewUniform = glGetUniformLocation(programID, "ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, GL_FALSE, glm::value_ptr(projectionViewMatrix));

	// set texture slot
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, perlinTexture);

	// tell the shader where it is
	projectionViewUniform = glGetUniformLocation(programID, "perlintexture");
	glUniform1i(projectionViewUniform, 0);

	// draw
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void ProcGen::shutdown()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}