#include "MyApplication.h"

Application::Application()
{
	white = vec4(1, 1, 1, 1);
	yellow = vec4(1, .8, .1, 1);
	blue = vec4(0, 0, 1, 1);
}

bool Application::startup()
{

	if (glfwInit() == false)
		return false;

	window = glfwCreateWindow(1280, 720, "The Milky Way", nullptr, nullptr);

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

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	glClearColor(0.0f, .0f, 0.0f, 1);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer

	return true;
}

bool Application::update()
{
	if (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Gizmos::clear();

		sun = glm::rotate(sun, (glm::mediump_float)180, glm::vec3(0, 1, 0));

		return true;
	}
	return false;
}

void Application::draw()
{
	Gizmos::addSphere(vec3(sun[3][0], sun[3][1], sun[3][2]), 3, 30, 30, yellow, &sun);
	Gizmos::addSphere(vec3(earth[3][0], earth[3][1], earth[3][2]), 1, 30, 30, blue, &earth);
	Gizmos::addSphere(vec3(moon[3][0], moon[3][1], moon[3][2]), .5, 30, 30, white, &moon);

	Gizmos::draw(projection * view);
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Application::shutdown()
{
	Gizmos::destroy();
	glfwDestroyWindow(window);
	glfwTerminate();
}
