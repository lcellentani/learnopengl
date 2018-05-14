#include "CommonDefine.h"
#include "GLApi.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "StringUtils.h"
#include "Camera.h"
#include "InputManager.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace
{

float gLastX = 0;
float gLastY = 0;
bool gFirstMouse = true;
bool gUseDirectional = true;

Camera gCamera;

}

void processInput(GLFWwindow *window, float deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		gCamera.ProcessKeyboard(Camera::Move::Forward, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		gCamera.ProcessKeyboard(Camera::Move::Backward, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		gCamera.ProcessKeyboard(Camera::Move::Left, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		gCamera.ProcessKeyboard(Camera::Move::Right, deltaTime);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	TINYNGINE_UNUSED(window);
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double posX, double posY) {
	TINYNGINE_UNUSED(window);
	if (gFirstMouse) {
		gLastX = float(posX);
		gLastY = float(posY);
		gFirstMouse = false;
	}

	float xOffset = float(posX) - gLastX;
	float yOffset = gLastY - float(posY);

	gLastX = float(posX);
	gLastY = float(posY);

	gCamera.ProcessMouse(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	TINYNGINE_UNUSED(window); TINYNGINE_UNUSED(xOffset);
	gCamera.ProcessMouseScroll(float(yOffset));
}

void UseDirectionalLight() {
	Log(tinyngine::Logger::Information, "SELECT LIGHT TYPE: DIRECTIONAL");
	gUseDirectional = true;
}

void UsePointLight() {
	Log(tinyngine::Logger::Information, "SELECT LIGHT TYPE: POINT");
	gUseDirectional = false;
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

	Input_Initialize(window);
	Input_BindKeyEvent(KeyEventType::Press, GLFW_KEY_1, UseDirectionalLight);
	Input_BindKeyEvent(KeyEventType::Press, GLFW_KEY_2, UsePointLight);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		Log(tinyngine::Logger::Error, "Failed to initialize GLAD");
		return 1;
	}

	ShaderProgramParams params;
	StringUtils::ReadFileToString("06-lights.vs", params.mVertexShaderData);
	StringUtils::ReadFileToString("06-lights.fs", params.mFragmentShaderData);
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

	TextureHandle textureHandle1 = Texture_Create("container2.png", TextureFormats::RGB8);
	if (!textureHandle1.IsValid()) {
		Log(tinyngine::Logger::Error, "Failed to create texture");
		return 1;
	}
	TextureHandle textureHandle2 = Texture_Create("container2_specular.png", TextureFormats::RGB8);
	if (!textureHandle2.IsValid()) {
		Log(tinyngine::Logger::Error, "Failed to create texture");
		return 1;
	}

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
	// positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	gCamera.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

	glm::vec4 lightPosition(1.2f, 1.0f, 2.0f, 1.0f);
	glm::vec4 lightDirection(-0.2f, -1.0f, -0.3f, 0.0);
	float lastFrameTime = 0.0f;
	float aspectRation = float(cScreenWidth) / float(cScreenHeight);

	glEnable(GL_DEPTH_TEST);

	glm::mat4 model;
	glm::mat4 modelView;
	glm::mat4 modelViewProj;
	glm::mat4 projection = glm::perspective(glm::radians(gCamera.GetFOV()), aspectRation, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(window)) {
		float currentFrameTime = float(glfwGetTime());
		float deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		glm::mat4 view = gCamera.GetViewMatrix();
		
		processInput(window, deltaTime);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Texture_Bind(textureHandle1, 0);
		Texture_Bind(textureHandle2, 1);

		ShaderProgram_Use(programHandle);
		ShaderProgram_SetInt(programHandle, "u_material.diffuse", 0);
		ShaderProgram_SetInt(programHandle, "u_material.specular", 1);
		ShaderProgram_SetFloat(programHandle, "u_material.shininess", 32.0f);
		
		ShaderProgram_SetVec4(programHandle, "u_light.direction", (gUseDirectional ? lightDirection : lightPosition));
		ShaderProgram_SetVec3(programHandle, "u_light.ambient", 0.01f, 0.01f, 0.01f);
		ShaderProgram_SetVec3(programHandle, "u_light.diffuse", 1.0f, 1.0f, 0.8f);
		ShaderProgram_SetVec3(programHandle, "u_light.specular", 1.0f, 1.0f, 1.0f);
		ShaderProgram_SetFloat(programHandle, "u_light.constant", 1.0f);
		ShaderProgram_SetFloat(programHandle, "u_light.linear", 0.09f);
		ShaderProgram_SetFloat(programHandle, "u_light.quadratic", 0.032f);
		
		ShaderProgram_SetVec3(programHandle, "u_viewPosition", gCamera.GetPosition());

		glBindVertexArray(cubeVAO);
		for (uint32_t i = 0; i < 10; i++) {
			float angle = 20.0f * i;
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			modelView = view * model;
			modelViewProj = projection * view * model;

			ShaderProgram_SetMat4(programHandle, "u_model", model);
			ShaderProgram_SetMat4(programHandle, "u_modelView", modelView);
			ShaderProgram_SetMat4(programHandle, "u_modelViewProj", modelViewProj);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(lightPosition.x, lightPosition.y, lightPosition.z));
		model = glm::scale(model, glm::vec3(0.2f));
		modelViewProj = projection * view * model;
		ShaderProgram_Use(lightProgramHandle);
		ShaderProgram_SetMat4(lightProgramHandle, "u_modelViewProj", modelViewProj);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &lightVAO);
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &VBO);
	Texture_Destroy(textureHandle2);
	Texture_Destroy(textureHandle1);
	ShaderProgram_Destroy(lightProgramHandle);
	ShaderProgram_Destroy(programHandle);

	glfwTerminate();
	return 0;
}
 