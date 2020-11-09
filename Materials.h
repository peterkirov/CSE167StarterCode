#pragma once
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Materials {
private:
	glm::vec3 ambient, diffuse, specular;
	float shininess;
public:
	Materials(glm::vec3 newAmbient, glm::vec3 newDiffuse, glm::vec3 newSpecular, float newShininess);
	void sendMatToShader(const int shaderID);
};