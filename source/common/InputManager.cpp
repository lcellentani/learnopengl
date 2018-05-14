#include "InputManager.h"
#include "Log.h"

#include <unordered_map>

namespace {

struct InputManagerImpl {
	std::unordered_map<int, std::vector<KeyEventCallback>> mKeysPressEventBindings;
	std::unordered_map<int, std::vector<KeyEventCallback>> mKeysReleaseEventBindings;
	std::unordered_map<int, std::vector<KeyEventCallback>> mKeysRepeatEventBindings;
};

InputManagerImpl gInputManager;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	TINYNGINE_UNUSED(window); TINYNGINE_UNUSED(scancode); TINYNGINE_UNUSED(mods);
	if (action == GLFW_PRESS) {
		Log(tinyngine::Logger::Information, ">>> KEY_PRESS: %d", key);
		for (auto event : gInputManager.mKeysPressEventBindings[key]) {
			event();
		}
	} else if (action == GLFW_REPEAT) {
		Log(tinyngine::Logger::Information, ">>> KEY_REPEAT: %d", key);
		for (auto event : gInputManager.mKeysRepeatEventBindings[key]) {
			event();
		}
	} else if (action == GLFW_RELEASE) {
		Log(tinyngine::Logger::Information, ">>> KEY_RELEASE: %d", key);
		for (auto event : gInputManager.mKeysReleaseEventBindings[key]) {
			event();
		}
	}
}

}

void Input_Initialize(GLFWwindow * window) {
	glfwSetKeyCallback(window, key_callback);
}

void Input_BindKeyEvent(KeyEventType::Enum type, int key, KeyEventCallback callback) {
	if ((type & KeyEventType::Press) != 0) {
		gInputManager.mKeysPressEventBindings[key].push_back(callback);
	}
	if ((type & KeyEventType::Repeat) != 0) {
		gInputManager.mKeysRepeatEventBindings[key].push_back(callback);
	}
	if ((type & KeyEventType::Release) != 0) {
		gInputManager.mKeysReleaseEventBindings[key].push_back(callback);
	}
}