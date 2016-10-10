#include "MyApplication.h"

CameraApplication::CameraApplication()
{
}

bool CameraApplication::startup()
{
	white = vec4(1, 0, 0, 1);
	black = vec4(0, 0, 0, 1);

	if (glfwInit() == false)
		return false;

	window = glfwCreateWindow(1280, 720, "Anthony is the Best!!", nullptr, nullptr);

	if (window == nullptr) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	myCamera.setLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	myCamera.setPerspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	myCamera.setSpeed(10);

	glEnable(GL_DEPTH_TEST);

	Gizmos::create();

	return true;
}

bool CameraApplication::update()
{
	currentTime = (float)glfwGetTime();
	deltaTime = currentTime - previousTime;
	previousTime = currentTime;

	if (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Gizmos::clear();

		return true;
	}
	return false;
}

void CameraApplication::draw()
{

	Gizmos::addTransform(glm::mat4(1));
	vec4 white(1);
	vec4 black(.25, .25, .25, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}

	myCamera.update(deltaTime, window);
	Gizmos::draw(myCamera.getProjectionView());

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void CameraApplication::shutdown()
{
	Gizmos::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
}