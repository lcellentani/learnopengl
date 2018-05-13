#include "CommonDefine.h"
#include "GLApi.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "StringUtils.h"

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	TINYNGINE_UNUSED(window);
	glViewport(0, 0, width, height);
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		Log(tinyngine::Logger::Error, "Failed to create GLFW window");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		Log(tinyngine::Logger::Error, "Failed to initialize GLAD");
		return 1;
	}
	
	ShaderProgramParams params;
	StringUtils::ReadFileToString("02-textures.vs", params.mVertexShaderData);
	StringUtils::ReadFileToString("02-textures.fs", params.mFragmentShaderData);
	ShaderProgramHandle programHandle = ShaderProgram_Create(params);
	if (!programHandle.IsValid()) {
		Log(tinyngine::Logger::Error, "Failed to create shader program");
		return 1;
	}

	TextureHandle textureHandle1 = Texture_Create("container.jpg", TextureFormats::RGB8);
	if (!textureHandle1.IsValid()) {
		Log(tinyngine::Logger::Error, "Failed to create texture");
		return 1;
	}
	TextureHandle textureHandle2 = Texture_Create("awesomeface.png", TextureFormats::RGBA8);
	if (!textureHandle2.IsValid()) {
		Log(tinyngine::Logger::Error, "Failed to create texture");
		return 1;
	}

	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Texture_Bind(textureHandle1, 0);
		Texture_Bind(textureHandle2, 1);

		ShaderProgram_Use(programHandle);
		ShaderProgram_SetInt(programHandle, "u_texture1", 0);
		ShaderProgram_SetInt(programHandle, "u_texture2", 1);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	Texture_Destroy(textureHandle2);
	Texture_Destroy(textureHandle1);
	ShaderProgram_Destroy(programHandle);

	glfwTerminate();
	return 0;
}
 