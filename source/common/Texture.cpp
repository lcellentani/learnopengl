#include "Texture.h"

#include "GLApi.h"
#include <array>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace
{

struct TextureFormatInfo {
	GLenum mInternalFormat;
	GLenum mFormat;
	GLenum mType;
};

static TextureFormatInfo sTextureFormats[]{
	{ GL_RGB, GL_RGB, GL_UNSIGNED_BYTE },			// RGB8
	{ GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE },			// RGBA8
};

class Texture {
public:
	Texture() = default;
	~Texture() {
		Destroy();
	}

	void Create(uint32_t width, uint32_t height, TextureFormats::Enum textureFormat, uint8_t* data) {
		GLenum internalFormat = sTextureFormats[textureFormat].mInternalFormat;
		GLenum format = sTextureFormats[textureFormat].mFormat;
		GLenum type = sTextureFormats[textureFormat].mType;

		GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
		glGenTextures(1, &mId);
		GL_ERROR(mId == 0);
		
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, mId));

		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data));
		GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));

		GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

		mWidth = width;
		mHeight = height;
	}

	void Destroy() {
		if (IsValid()) {
			GL_CHECK(glBindTexture(GL_TEXTURE_2D, mId));
			GL_CHECK(glDeleteTextures(1, &mId));
			GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
			mId = 0;
		}
	}

	void Bind(uint8_t stage = 0) {
		if (IsValid()) {
			GL_CHECK(glActiveTexture(GL_TEXTURE0 + stage));
			GL_CHECK(glBindTexture(GL_TEXTURE_2D, mId));
		}
	}

	bool IsValid() const {
		return mId > 0;
	}

private:
	GLuint mId = 0;
	uint32_t mWidth = 0;
	uint32_t mHeight = 0;
};

static constexpr uint32_t cMaxTextureHandles = (1 << 6);
uint32_t sTexturesCount = 0;
std::array<Texture, cMaxTextureHandles> sTextures;

}

TextureHandle Texture_Create(const char* filename, TextureFormats::Enum format) {
	if (filename != nullptr) {
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(filename, &width, &height, &channels, 0);
		TINYNGINE_UNUSED(channels);
		if (data) {
			TextureHandle handle = TextureHandle(sTexturesCount);
			auto& texture = sTextures[handle.mHandle];
			texture.Create(width, height, format, data);

			stbi_image_free(data);
			sTexturesCount++;

			return handle;
		}
	}
	return TextureHandle(cInvalidHandle);
}

void Texture_Destroy(const TextureHandle& handle) {
	if (!handle.IsValid()) {
		return;
	}
	auto& texture = sTextures[handle.mHandle];
	texture.Destroy();
}

void Texture_Bind(const TextureHandle& handle, uint8_t stage) {
	if (!handle.IsValid()) {
		return;
	}
	auto& texture = sTextures[handle.mHandle];
	texture.Bind(stage);
}

void Texture_SetFilteringMode(const TextureHandle & handle, TextureFilteringMode::Enum mode) {
	(void)handle; (void)mode;
}

void Texture_SetWrappingMode(const TextureHandle & handle, TextureWrapMode::Enum mode) {
	(void)handle; (void)mode;
}
