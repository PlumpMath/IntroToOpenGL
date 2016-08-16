#include "src\gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <iostream>
#define GLM_SWIZZLE
#define GLM_FORCE_PURE
#include "src/Gizmos.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

int main()
{

	if (glfwInit() == false)
		return -1;

	GLFWwindow* window = glfwCreateWindow(1280, 720, "SPAAAAAAAAAACE!", nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}

	Gizmos::create();

	mat4 view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.0f);

	mat4 model = mat4(1); // identity matrix

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	glClearColor(0.0f, .0f, 0.0f, 1);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer

	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Gizmos::clear();

		//Gizmos::addTransform(glm::mat4(1)); // Transform marker

		//for (int i = 0; i < 21; ++i) 
		//{
		//	Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);
		//	Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
		//}

		vec4 white(1, 1, 1, 1);
		vec4 black(0, 0, 0, 1);
		vec4 yellow(1, .8, .1, 1);
		vec4 blue(0, 0, 1, 1);
		vec4 green(0, .8, .1, 1);

		vec3 bluePosition = vec3(5, 3, 1);
		vec3 greenPosition = vec3(3, 5, 1);

		model = glm::rotate(model, (glm::mediump_float)180, glm::vec3(0, 1, 0));

		Gizmos::addSphere(vec3(0), 2, 30, 30, yellow);
		Gizmos::addSphere(bluePosition, 0.5f, 20, 20, blue, &model);
		Gizmos::addSphere(greenPosition, 0.5f, 20, 20, green, &model);

		Gizmos::draw(projection * view);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Gizmos::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}