#include <Skybox.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/** Shader Wrapper */
#include <ShaderProgram.h>

/** Shader Wrapper */
#include <Texture.h>

std::vector<float> skyboxVertices = {
	// front
	-1.0, -1.0,  1.0,
	 1.0, -1.0,  1.0,
	 1.0,  1.0,  1.0,
	-1.0,  1.0,  1.0,
	// back
	-1.0, -1.0, -1.0,
	 1.0, -1.0, -1.0,
	 1.0,  1.0, -1.0,
	-1.0,  1.0, -1.0,
};

std::vector<unsigned int> skyboxElements = {
	// right
	1, 5, 6,
	6, 2, 1,
	// left
	4, 0, 3,
	3, 7, 4,
	// top
	3, 2, 6,
	6, 7, 3,
	// bottom
	4, 5, 1,
	1, 0, 4,
	// front
	0, 1, 2,
	2, 3, 0,
	// back
	7, 6, 5,
	5, 4, 7,
};

int texture_skybox_index = 15;

Skybox :: Skybox() { setup(); }

void Skybox :: setup() {

	glGenBuffers(1, &vbo); // Generate an empty vertex buffer on the GPU
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao); // Tell OpenGL to create new Vertex Array Object
	
	glBindVertexArray(vao); // Make the vertices buffer the current one
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // "bind" or set as the current buffer we are working with
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * skyboxVertices.size(),
		skyboxVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * skyboxElements.size(),
		skyboxElements.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

	glBindVertexArray(0); // Release control of vao
}

void Skybox :: Draw(ShaderProgram & shader, glm::mat4 & view, glm::mat4 & projection) {

	shader.use();
	shader.setUniform("uView", view);
	shader.setUniform("uProjection", projection);
	shader.setUniform("uSkybox", 0); // no other texture units

	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL); // change depth func so depth test passes when val == depth buffer

	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0 + texture_skybox_index);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tid);
	glDrawElements(GL_TRIANGLES, skyboxElements.size(), GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0); // Unbind
	glActiveTexture(GL_TEXTURE0);
	glDepthFunc(GL_LESS); // restore default depth func
	glDepthMask(GL_TRUE);
}

void Skybox :: LoadTexture(std::vector<std::string> & faces) {
	tid = LoadCubemap(faces);
}
