#include "src\gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <vector>
#define GLM_SWIZZLE
#define GLM_FORCE_PURE
#include "src/Gizmos.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <Camera.h>

using glm::vec2;
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

struct Vertex 
{
	vec4 position;
	vec4 colour;
};

struct AdvancedVertex 
{
	float x, y, z, w;
	float nx, ny, nz, nw;
	float tx, ty, tz, tw;
	float s, t;
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

class CameraApplication : public Application
{
public:
	CameraApplication();
	bool startup() override;
	bool update() override;
	void draw() override;
	void shutdown() override;

private:
	GLFWwindow* window;
	FlyCamera myCamera;
	vec4 white;
	vec4 black;
	float previousTime = 0;
	float currentTime;
	float deltaTime;
};

class RenderGeo : public Application
{
public :
	RenderGeo();
	bool startup() override;
	bool update() override;
	void draw() override;
	void shutdown() override;
	void makeShader();
	void makePlane();
	Vertex* makeHalfCircle(const int, const int);
	Vertex* makeSphereVerts(const int, const int, Vertex*);
	void makeSphere(const int, const int, const int);
	//std::string readFile(std::string text);
	
private:
	mat4 projection;
	mat4 view;
	mat4 projectionViewMatrix;
	GLFWwindow* window;
	unsigned int VAO; // vertex array object
	unsigned int VBO; // vertex buffer object
	unsigned int IBO; // index buffer object
	unsigned int indexCount;
	unsigned int programID; // resulting ID of compiled shader
	float time;
	float radius;
	float phi;
	float newX;
	float newY;
	float newZ;
};

class Textures : public Application
{
public:
	Textures();
	bool startup() override;
	bool update() override;
	void draw() override;
	void shutdown() override;

private:
	mat4 projection;
	mat4 view;
	mat4 projectionViewMatrix;
	GLFWwindow* window;
	unsigned int VAO; // vertex array object
	unsigned int VBO; // vertex buffer object
	unsigned int IBO; // index buffer object
	unsigned int texture;
	unsigned int normal;
	unsigned int programID;
};

class ProcGen : public Application
{
public:
	ProcGen();
	bool startup() override;
	bool update() override;
	void draw() override;
	void shutdown() override;

private:
	mat4 projection;
	mat4 view;
	mat4 projectionViewMatrix;
	GLFWwindow* window;
	unsigned int VAO; // vertex array object
	unsigned int VBO; // vertex buffer object
	unsigned int IBO; // index buffer object
	unsigned int perlinTexture;
	unsigned int programID;
};