#pragma once

#include "CommonDefine.h"
#include "GLApi.h"

struct KeyEventType {
	enum Enum {
		Press = 1 << 0,
		Release = 1 << 1,
		Repeat = 1 << 2,
		PressAndRepeat = Press | Repeat
	};
};

using KeyEventCallback = std::function<void()>;
//using MouseEventCallback = std::function<void()>;


void Input_Initialize(GLFWwindow *window);
void Input_BindKeyEvent(KeyEventType::Enum type, int key, KeyEventCallback callback);
//void BindMouseEvent(mode, func);
