#include "Window.h"


// Window Properties
int Window::width;
int Window::height;
float Window::phong = 0.0;
float Window::Mode = 1;
bool Window::isHeld;
glm::vec3 Window::lastPoint;
double Window::pos_x;
double Window::pos_y;
const char* Window::windowTitle = "GLFW Starter Project";
PointLight* Window::pl;
//materials for the objects
Materials* Window::bunnyMaterial;
Materials* Window::sandalMaterial;
Materials* Window::bearMaterial;
Materials* Window::sphereMaterial;

// Objects to Render
Cube * Window::cube;
PointCloud * Window::cubePoints;
PointCloud* Window::bunnyPoints;
PointCloud* Window::sandalPoints;
PointCloud* Window::bearPoints;
PointCloud* Window::spherePoints;
Object* currObj;

// Camera Matrices 
// Projection matrix:
glm::mat4 Window::projection; 

// View Matrix:
glm::vec3 Window::eyePos(0, 0, 20);			// Camera position.
glm::vec3 Window::lookAtPoint(0, 0, 0);		// The point we are looking at.
glm::vec3 Window::upVector(0, 1, 0);		// The up direction of the camera.
glm::mat4 Window::view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);

// Shader Program ID
GLuint Window::shaderProgram; 



bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	// Check the shader program.
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects()
{
	// Create a cube of size 5.
	cube = new Cube(5.0f);
	bunnyMaterial = new Materials(glm::vec3(.2), glm::vec3(1), glm::vec3(0), 32);
	bearMaterial = new Materials(glm::vec3(.2), glm::vec3(0.2), glm::vec3(1), 32);
	sandalMaterial = new Materials(glm::vec3(.2), glm::vec3(1), glm::vec3(1), 32);
	sphereMaterial = new Materials(glm::vec3(0.7, 0.7, 0.2), glm::vec3(0), glm::vec3(0), 32);
	

	// Create a point cloud consisting of cube vertices.
	
	spherePoints = new PointCloud("sphere.obj", 1, sphereMaterial);
	spherePoints->scaleObj(glm::vec3(1.0 / 32.0));
	spherePoints->translateObj(glm::vec3(4.0));
	bunnyPoints = new PointCloud("bunny.obj", 10, bunnyMaterial);
	sandalPoints = new PointCloud("SandalF20.obj", 1, sandalMaterial);
	bearPoints = new PointCloud("bear.obj", 1, bearMaterial);
	pl = new PointLight(spherePoints,glm::vec3(4), glm::vec3(0.7, 0.7, 0.2), glm::vec3(1));
	currObj = bunnyPoints;

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete cube;
	//delete cubePoints;
	delete pl;
	delete spherePoints;
	delete bunnyPoints;
	delete sandalPoints;
	delete bearPoints;
	delete bunnyMaterial;
	delete bearMaterial;
	delete sandalMaterial;
	delete sphereMaterial;

	// Delete the shader program.
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(60.0), 
								double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback()
{
	// Perform any necessary updates here 
	//currObj->update();
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	//pass mats and light to shader
	//send info to shader
	//
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, GL_FALSE, glm::value_ptr(eyePos));
	// Render the objects
	pl->sendLightToShader(shaderProgram);
	spherePoints->draw(view, projection, shaderProgram);
	currObj->draw(view, projection, shaderProgram);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;
		case  GLFW_KEY_F1:
			currObj = bunnyPoints;
			break;
		case  GLFW_KEY_F2:
			currObj = sandalPoints;
			break;
		case  GLFW_KEY_F3:
			currObj = bearPoints;
			break;
		case  GLFW_KEY_S:
			currObj->updatePointSize(-2);
			break;
		case  GLFW_KEY_L:
			currObj->updatePointSize(2);
			break;
		case GLFW_KEY_N:
			if (phong == 0.0) {
				phong = 1.0;
				glUniform1f(glGetUniformLocation(shaderProgram, "phong"), phong);
			}
			else {
				phong = 0.0;
				glUniform1f(glGetUniformLocation(shaderProgram, "phong"), phong);
			}

			break;
		case GLFW_KEY_1:
			Mode = 1;
			break;
		case GLFW_KEY_2:
			Mode = 2;
			break;
		case GLFW_KEY_3:
			Mode = 3;
			break;
		default:
			break;
		}
	}
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		isHeld = true;
		lastPoint = trackBallMapping(pos_x, pos_y);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		isHeld = false;
	}
}


void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	pos_x = xpos;
	pos_y = ypos;

	glm::vec3 direction, curPoint;
	float rot_angle;
	if (Mode == 1) {
		if (isHeld) {
			curPoint = trackBallMapping(pos_x, pos_y);
			direction = curPoint - lastPoint;
			float velocity = glm::length(direction);
			if (velocity > 0.0001) {
				glm::vec3 rotAxis;
				rotAxis = glm::cross(lastPoint, curPoint);
				rot_angle = velocity * 5;
				currObj->update(rotAxis, rot_angle);
			}
		}
		lastPoint = curPoint;
	}
	else if (Mode == 2) {
		if (isHeld) {
			curPoint = trackBallMapping(pos_x, pos_y);
			direction = curPoint - lastPoint;
			float velocity = glm::length(direction);
			if (velocity > 0.0001) {
				glm::vec3 rotAxis;
				rotAxis = glm::cross(lastPoint, curPoint);
				rot_angle = velocity * 5;
				pl->updateLight(rotAxis, rot_angle);
			}
		}
		lastPoint = curPoint;
	}
	else if (Mode == 3) {
		if (isHeld) {
			curPoint = trackBallMapping(pos_x, pos_y);
			direction = curPoint - lastPoint;
			float velocity = glm::length(direction);
			if (velocity > 0.0001) {
				glm::vec3 rotAxis;
				rotAxis = glm::cross(lastPoint, curPoint);
				rot_angle = velocity * 5;
				pl->updateLight(rotAxis, rot_angle);
				currObj->update(rotAxis, rot_angle);
			}
		}
		lastPoint = curPoint;
	}
}


void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	double y_off = yoffset;
	if (Mode == 1) {
		if (y_off > 0) {
			currObj->scaleObj(glm::vec3(1.3));
		}
		else {
			currObj->scaleObj(glm::vec3(1 / 1.3));
		}
	}
	else if (Mode == 2) {
		if (y_off > 0) {
			pl->translateLight(glm::vec3(0.1));
		}
		else {
			pl->translateLight(glm::vec3(-0.1));
		}
	}
	else if (Mode == 3) {
		if (y_off > 0) {
			currObj->scaleObj(glm::vec3(1.3));
			pl->translateLight(glm::vec3(0.1));
		}
		else {
			currObj->scaleObj(glm::vec3(1 / 1.3));
			pl->translateLight(glm::vec3(-0.1));
		}
	}
}

glm::vec3 Window::trackBallMapping(double xpos, double ypos)
{
	glm::vec3 v;
	float d;
	v.x = (2.0 * xpos - width) /width;
	v.y = (height - 2.0 * ypos) / height;
	v.z = 0.0;
	d = glm::length(v);
	d = (d < 1.0) ? d : 1.0;
	v.z = sqrtf(1.001 - d * d);
	v = glm::normalize(v); // Still need to normalize, since we only capped d, not v.
	return v;
}
