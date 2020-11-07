#include "PointCloud.h"



PointCloud::PointCloud(std::string objFilename, GLfloat pointSize) 
	: pointSize(pointSize)
{
	/* 
	 * TODO: Section 2: Currently, all the points are hard coded below. 
	 * Modify this to read points from an obj file.
	 */
	
	/*points = {
		glm::vec3(-2.5, 2.5, 2.5),
		glm::vec3(-2.5, -2.5, 2.5),
		glm::vec3(2.5, -2.5, 2.5),
		glm::vec3(2.5, 2.5, 2.5),
		glm::vec3(-2.5, 2.5, -2.5),
		glm::vec3(-2.5, -2.5, -2.5),
		glm::vec3(2.5, -2.5, -2.5),
		glm::vec3(2.5, 2.5, -2.5)
	};
	*/
	std::ifstream objFile(objFilename); // The obj file we are reading.
	//std::vector<glm::vec3> points;

	// Check whether the file can be opened.
	if (objFile.is_open())
	{
		std::string line; // A line in the file.

		// Read lines from the file.
		while (std::getline(objFile, line))
		{
			// Turn the line into a string stream for processing.
			std::stringstream ss;
			ss << line;

			// Read the first word of the line.
			std::string label;
			ss >> label;

			// If the line is about vertex (starting with a "v").
			if (label == "v")
			{
				// Read the later three float numbers and use them as the 
				// coordinates.
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;

				// Process the points.
				points.push_back(point);
			}
			/*
			* f 31514//31514 31465//31465 31464//31464
			* glm::uvec3
			* glm::ivec3
			* vn 2.095934 0.202524 0.494661
			* windowSize.x = 640
			* windowSize.y = 480
			*/
			if (label == "vn") {
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;

				// Process the normals.
				normal.push_back(point);
			}
			if (label == "f") {
				glm::uvec3 face;
				ss >> face.x;
				ss.ignore(256, ' ');
				face.x -= 1;
				ss >> face.y;
				face.y -= 1;
				ss.ignore(256, ' ');

				ss >> face.z;
				face.z -= 1;
				indices.push_back(face);
			}
		}
	}
	else
	{
		std::cerr << "Can't open the file " << objFilename << std::endl;
	}

	objFile.close();
	/*
	 * TODO: Section 4, you will need to normalize the object to fit in the
     * screen.
     */

	//init maxs and mins
	float xMax = -100000000.;
	float xMin = 100000000.;
	float yMax = -100000000.;
	float yMin = 100000000.;
	float zMax = -100000000.;
	float zMin = 100000000.;

	//find the max and min for each x,y,z
	for (auto& point : points) {
		if (point.x > xMax) {
			xMax = point.x;
		}
		if (point.x < xMin) {
			xMin = point.x;
		}
		if (point.y > yMax) {
			yMax = point.y;
		}
		if (point.y < yMin) {
			yMin = point.y;
		}
		if (point.z > zMax) {
			zMax = point.z;
		}
		if (point.z < zMin) {
			zMin = point.z;
		}
	}
	//calc mid point
	float midx = (xMax + xMin) / 2;
	float midy = (yMax + yMin) / 2;
	float midz = (zMax + zMin) / 2;
	//init midpoint
	glm::vec3 point;
	point.x = midx;
	point.y = midy;
	point.z = midz;
	//subtract each point by the midpoint
	for (auto& point : points) {
		point.x -= midx;
		point.y -= midy;
		point.z -= midz;
	}
	//get the scale factor
	float sFactor = std::max(xMax-xMin, yMax - yMin );
	sFactor = std::max(sFactor, zMax - zMin);
	for (auto& point : points) {
		point.x /= sFactor;
		point.y /= sFactor;
		point.z /= sFactor;
	}

	float upFactor = 15.4;
	for (auto& point : points) {
		point.x *= upFactor;
		point.y *= upFactor;
		point.z *= upFactor;
	}
	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);

	// Set the color. 
	color = glm::vec3(1, 0, .5);

	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO1);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	// Bind VBO1 to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* normal.size(), normal.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	// Generate EBO, bind the EBO to the bound VAO, and send the index data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec3)* indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

PointCloud::~PointCloud() 
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void PointCloud::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader)
{
	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));

	// Bind the VAO
	glBindVertexArray(VAO);



	// Draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, indices.size()*3, GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void PointCloud::update(glm::vec3 rotAxis, GLfloat rot_angle)
{
	model = glm::rotate(model, rot_angle, rotAxis);
}

void PointCloud::updatePointSize(GLfloat size) 
{
	/*
	 * TODO: Section 3: Implement this function to adjust the point size.
	 */
	pointSize += size;

}
void PointCloud::scaleObj(glm::vec3 size)
{
	glm::mat4 scaler = glm::scale(size);
	model *= scaler;

}
void PointCloud::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}
