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

class Application
{

public:

	Application();
	bool startup();
	bool update();
	void draw();
	void shutdown();

private:

	mat4 projection;
	mat4 view;
	vec4 white;
	vec4 yellow;
	vec4 blue;
	GLFWwindow* window;
	mat4 sun = mat4(1);
	mat4 earth = mat4(1);
	mat4 moon = mat4(1);
	float angle = 0;

};