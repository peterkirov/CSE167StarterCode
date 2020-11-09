#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#include "Object.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class PointCloud : public Object
{
private:
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normal;
	std::vector<glm::uvec3> indices;
	GLuint VAO, VBO, VBO1, EBO;
	GLfloat pointSize;
	Materials* material;
public:
	PointCloud(std::string objFilename, GLfloat pointSize, Materials* newMaterial);
	~PointCloud();
	
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update(glm::vec3 rotAxis, GLfloat rot_angle);
	void updatePointSize(GLfloat size);
	void scaleObj(glm::vec3 size);
	void translateObj(glm::vec3 size);
	void spin(float deg);
};

#endif
