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
	virtual bool startup() = 0;
	virtual bool update() = 0;
	virtual void draw() = 0;
	virtual void shutdown() = 0;
};

class SolarSystem : public Application
{
public:
	SolarSystem();
	bool startup() override;
	bool update() override;
	void draw() override;
	void shutdown() override;
private:
	mat4 projection;
	mat4 view;
	vec4 white;
	vec4 yellow;
	vec4 green;
	vec4 black;
	float previousTime;
	float currentTime;
	float deltaTime;
	GLFWwindow* window;
	mat4 sun = mat4(1); // identity matrices
	mat4 earth = mat4(1); // ^
	mat4 moon = mat4(1); //  ^
	float angle = 0; // rotation angle
	vec3 earthOffset = vec3(7, 0, 0); // distance of earth from sun
	vec3 moonOffset = vec3(3, 0, 0); // distance of moon from earth
};

class RenderGeo : public Application
{
public :
	RenderGeo();
	bool startup() override;
	bool update() override;
	void draw() override;
	void shutdown() override;
	void makePlane();
	
private:
	struct Vertex 
	{
		vec4 position;
		vec4 color;
	};
	mat4 projection;
	mat4 view;
	GLFWwindow* window;
	unsigned int VAO; // vertex array object
	unsigned int VBO; // vertex buffer object
	unsigned int IBO; // index buffer object
	unsigned int indexCount;
	unsigned int programID; // resulting ID of compiled shader
};