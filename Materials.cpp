#include "Materials.h"

Materials::Materials(glm::vec3 newAmbient, glm::vec3 newDiffuse, glm::vec3 newSpecular, float newShininess)
{
	ambient = newAmbient;
	diffuse = newDiffuse;
	specular = newSpecular;
	shininess = newShininess;
}

void Materials::sendMatToShader(const int shaderID)
{
	glUniform3fv(glGetUniformLocation(shaderID, "ambient"), 1, glm::value_ptr(ambient));
	glUniform3fv(glGetUniformLocation(shaderID, "diffuse"), 1, glm::value_ptr(diffuse));
	glUniform3fv(glGetUniformLocation(shaderID, "specularpassed"), 1, glm::value_ptr(specular));
	glUniform1f(glGetUniformLocation(shaderID, "shininess"), shininess * 128);
}