#ifndef _CUBE_H_
#define _CUBE_H_

#include "Object.h"

#include <vector>

class Cube : public Object
{
private:
	GLuint VAO;
	GLuint VBO, EBO;

public:
	Cube(float size);
	~Cube();

	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update(glm::vec3 rotAxis, GLfloat rot_angle);
	void updatePointSize(GLfloat size) {}
	void scaleObj(glm::vec3 size) {}
	void spin(float deg);
};

#endif