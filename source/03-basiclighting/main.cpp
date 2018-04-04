#include "CommonDefine.h"
#include "GLApi.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "StringUtils.h"
#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace
{

float slastX = 0;
float sLastY = 0;
bool sFirstMouse = true;

Camera sCamera;

}

void processInput(GLFWwindow *window, float deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		sCamera.ProcessKeyboard(Camera::Move::Forward, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		sCamera.ProcessKeyboard(Camera::Move::Backward, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		sCamera.ProcessKeyboard(Camera::Move::Left, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		sCamera.ProcessKeyboard(Camera::Move::Right, deltaTime);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	TINYNGINE_UNUSED(window);
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double posX, double posY) {
	TINYNGINE_UNUSED(window);
	if (sFirstMouse) {
		slastX = float(posX);
		sLastY = float(posY);
		sFirstMouse = false;
	}

	float xOffset = float(posX) - slastX;
	float yOffset = sLastY - float(posY);

	slastX = float(posX);
	sLastY = float(posY);

	sCamera.ProcessMouse(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	TINYNGINE_UNUSED(window); TINYNGINE_UNUSED(xOffset);
	sCamera.ProcessMouseScroll(float(yOffset));
}

int main() {
	const uint32_t cScreenWidth = 800;
	const uint32_t cScreenHeight = 600;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	GLFWwindow* window = glfwCreateWindow(cScreenWidth, cScreenHeight, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		Log(tinyngine::Logger::Error, "Failed to create GLFW window");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		Log(tinyngine::Logger::Error, "Failed to initialize GLAD");
		return 1;
	}
	
	ShaderProgramParams params;
	StringUtils::ReadFileToString("03-basiclighting.vs", params.mVertexShaderData);
	StringUtils::ReadFileToString("03-basiclighting.fs", params.mFragmentShaderData);
	ShaderProgramHandle programHandle = ShaderProgram_Create(params);
	if (!programHandle.IsValid()) {
		Log(tinyngine::Logger::Error, "Failed to create shader program");
		return 1;
	}

	StringUtils::ReadFileToString("dbg_light.vs", params.mVertexShaderData);
	StringUtils::ReadFileToString("dbg_light.fs", params.mFragmentShaderData);
	ShaderProgramHandle lightProgramHandle = ShaderProgram_Create(params);
	if (!lightProgramHandle.IsValid()) {
		Log(tinyngine::Logger::Error, "Failed to create shader program");
		return 1;
	}

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	unsigned int cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	sCamera.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

	glm::vec3 lightPosition(1.2f, 1.0f, 2.0f);
	float lastFrameTime = 0.0f;
	float aspectRation = float(cScreenWidth) / float(cScreenHeight);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		float currentFrameTime = float(glfwGetTime());
		float deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		glm::mat4 projection = glm::perspective(glm::radians(sCamera.GetFOV()), aspectRation, 0.1f, 100.0f);
		glm::mat4 view = sCamera.GetViewMatrix();

		processInput(window, deltaTime);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ShaderProgram_Use(programHandle);
		ShaderProgram_SetVec3(programHandle, "u_objectColor", 1.0f, 0.5f, 0.31f);
		ShaderProgram_SetVec3(programHandle, "u_lightColor", 1.0f, 1.0f, 1.0f);
		ShaderProgram_SetVec3(programHandle, "u_lightPosition", lightPosition);
		ShaderProgram_SetMat4(programHandle, "u_projection", projection);
		ShaderProgram_SetMat4(programHandle, "u_view", view);
		glm::mat4 model(1.0f);
		ShaderProgram_SetMat4(programHandle, "u_model", model);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		ShaderProgram_Use(lightProgramHandle);
		ShaderProgram_SetMat4(lightProgramHandle, "u_projection", projection);
		ShaderProgram_SetMat4(lightProgramHandle, "u_view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPosition);
		model = glm::scale(model, glm::vec3(0.2f));
		ShaderProgram_SetMat4(lightProgramHandle, "u_model", model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &lightVAO);
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &VBO);
	ShaderProgram_Destroy(lightProgramHandle);
	ShaderProgram_Destroy(programHandle);

	glfwTerminate();
	return 0;
}
 