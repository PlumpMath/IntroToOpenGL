
#include <GLFW/glfw3.h>
int main()
{

}
//#include "MyApplication.h"
//
//RenderGeo::RenderGeo()
//{
//}
//
//bool RenderGeo::startup()
//{
//
//	if (glfwInit() == false)
//		return false;
//
//	window = glfwCreateWindow(1280, 720, "Rendering Geometry", nullptr, nullptr);
//
//	if (window == nullptr)
//	{
//		glfwTerminate();
//		return false;
//	}
//
//	glfwMakeContextCurrent(window);
//
//	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
//	{
//		glfwDestroyWindow(window);
//		glfwTerminate();
//		return false;
//	}
//
//	Gizmos::create(); // initializes
//
//	view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
//	projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.0f);
//
//	glClearColor(0.0f, .0f, 0.0f, 1);
//	glEnable(GL_DEPTH_TEST); // enables the depth buffer
//
//	return true;
//}
//
//bool RenderGeo::update()
//{
//	if (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
//	{
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		Gizmos::clear();
//
//		return true;
//	}
//	return false;
//}
//
//void RenderGeo::draw()
//{
//	Gizmos::draw(projection * view);
//	glfwSwapBuffers(window);
//	glfwPollEvents();
//}
//
//void RenderGeo::shutdown()
//{
//	Gizmos::destroy();
//	glfwDestroyWindow(window);
//	glfwTerminate();
//}