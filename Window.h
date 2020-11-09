#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "shader.h"
#include "Object.h"
#include "Cube.h"
#include "PointCloud.h"
#include "PointLight.h"
class Window
{
public:

	// Window Properties
	static int width;
	static int height;
	static float phong;
	static float Mode;
	static bool isHeld;
	static double pos_x, pos_y;
	static glm::vec3 lastPoint;
	static const char* windowTitle;
	static PointLight* pl;
	//materials for the objects
	static Materials* bunnyMaterial;
	static Materials* sandalMaterial;
	static Materials* bearMaterial;
	static Materials* sphereMaterial;
	// Objects to Render
	static Cube* cube;
	static PointCloud * cubePoints;
	static PointCloud * bunnyPoints;
	static PointCloud * sandalPoints;
	static PointCloud * bearPoints;
	static PointCloud * spherePoints;

	// Camera Matrices
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eyePos, lookAtPoint, upVector;

	// Shader Program ID
	static GLuint shaderProgram;

	// Constructors and Destructors
	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();

	// Window functions
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	// Draw and Update functions
	static void idleCallback();
	static void displayCallback(GLFWwindow*);

	// Callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static glm::vec3 trackBallMapping(double xpos, double ypos);
};

#endif
