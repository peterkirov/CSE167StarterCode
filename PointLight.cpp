#include "PointLight.h"

PointLight::PointLight(PointCloud* newPointCloud, glm::vec3 newPos = glm::vec3(4), glm::vec3 newColor = glm::vec3(0.7, 0.7, 0.2), glm::vec3 newAtten = glm::vec3(1))
{
	pos = newPos;
	color = newColor;
	atten = newAtten;
	pointSphere = newPointCloud;
}


void PointLight::sendLightToShader(const int shaderID)
{
	glUniform3fv(glGetUniformLocation(shaderID, "lightColor"), 1, glm::value_ptr(color));
	glUniform3fv(glGetUniformLocation(shaderID, "lightPos"), 1, glm::value_ptr(pos));
}

void PointLight::updateLight(glm::vec3 rotAxis, GLfloat rot_angle)
{
	pointSphere->update(rotAxis, rot_angle);
	pos = pointSphere->getModel()[3];
}

void PointLight::translateLight(glm::vec3 nice)
{

	pointSphere->translateObj(nice*pos);
	pos = pointSphere->getModel()[3];
}
