#pragma once

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "PointCloud.h"
class PointLight {
private:
	glm::vec3 pos, color, atten;
	

public:
	PointLight(PointCloud* newPointCloud, glm::vec3 newPos, glm::vec3 newColor, glm::vec3 newAtten);
	void sendLightToShader(const int shaderID);
	void updateLight(glm::vec3 rotAxis, GLfloat rot_angle);
	void translateLight(glm::vec3 nice);
	PointCloud* pointSphere;
};