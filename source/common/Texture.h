#pragma once

#include "CommonDefine.h"

struct TextureFormats {
	enum Enum {
		RGB8,
		RGBA8,
		Count
	};
};

struct TextureFilteringMode {
	enum Enum {
		None,
		Nearest,
		Bilinear,
		Trilinear,
		Count
	};
};

struct TextureWrapMode {
	enum Enum {
		Repeat,
		ClampToEdge,
		Mirrored,
		ClampToBorder,
		Count
	};
};

using TextureHandle = ResourceHandle;

TextureHandle Texture_Create(const char* filename, TextureFormats::Enum format);

void Texture_Destroy(const TextureHandle& handle);

void Texture_Bind(const TextureHandle& handle, uint8_t stage);

void Texture_SetFilteringMode(const TextureHandle& handle, TextureFilteringMode::Enum mode);

void Texture_SetWrappingMode(const TextureHandle& handle, TextureWrapMode::Enum mode);