#include "glrhi/core/texture2D.hpp"
#include <iostream>


namespace glrhi {

	texture2D::texture2D(GLsizei width, GLsizei height, GLenum internalFormat, GLsizei mips) {
		create(width, height, internalFormat, mips);
	}

	texture2D::texture2D(const std::filesystem::path& file, GLenum internalFormat, GLsizei mips) {
		create(file, internalFormat, mips);
	}

	void texture2D::create(GLsizei width, GLsizei height, GLenum internalFormat, GLsizei mips) {
		if (m_ID) {
			std::cerr << "ERROR::TEXTURE2D::TEXTURE_ALREADY_CREATED\n";
			return;
		}
		
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);

		// Set texture sampling to use mipmaps if there are any
		if (mips > 1)
			glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Texture wrapping options
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureStorage2D(m_ID, mips, internalFormat, width, height);

		m_width = width;
		m_height = height;
		m_format = internalFormat;
	}

	void texture2D::create(const std::filesystem::path& file, GLenum internalFormat, GLsizei mips) {
		if (m_ID) {
			std::cerr << "ERROR::TEXTURE2D::TEXTURE_ALREADY_CREATED\n";
			return;
		}
		
		// Texture creation
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);

		// Set texture sampling to use mipmaps if there are any
		if (mips > 1)
			glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Texture wrapping options
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Image properties
		int widthImg, heightImg, numColCh;
		GLenum dataType = GL_UNSIGNED_BYTE;
		bool hdr = false;
		void* data = nullptr;

		// check image properties
		if (stbi_is_hdr(file.c_str())) {
			hdr = true;
			dataType = GL_FLOAT;
		}

		// Loading image into memory
		stbi_set_flip_vertically_on_load(true);
		if (!hdr)
			data = stbi_load(file.c_str(), &widthImg, &heightImg, &numColCh, 4);
		else
			data = stbi_loadf(file.c_str(), &widthImg, &heightImg, &numColCh, 4);

		// Allocate memory on the gpu
		glTextureStorage2D(m_ID, mips, internalFormat, widthImg, heightImg);

		m_width = widthImg;
		m_height = heightImg;
		m_format = internalFormat;

		// load the image data to the gpu
		loadData(GL_RGBA, dataType, data);

		// generate mipmaps
		if (mips > 1) {
			glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			genMipmaps();
		}
		else {
			glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		// Free the memory used by stb.
		stbi_image_free(data);  
	}

	texture2D::~texture2D() {
		// Make handles non resident if they were used
		if (m_samplerHandle)
			glMakeTextureHandleNonResidentARB(m_samplerHandle);

		if (m_imageHandle)
			glMakeImageHandleNonResidentARB(m_imageHandle);

		// Delete the texture buffer
		glDeleteTextures(1, &m_ID);
	}

	void texture2D::loadData(GLenum dataFormat, 
									GLenum dataType, 
									const void* data, 
									GLint mip) const {

		glTextureSubImage2D(m_ID, mip, 0, 0, m_width, m_height, dataFormat, dataType, data);

	}

	void texture2D::genMipmaps() const {
		glGenerateTextureMipmap(m_ID);
	}

	void texture2D::bind(GLuint textureUnit) const {
		glBindTextureUnit(textureUnit, m_ID);
	}

	void texture2D::bindImage(GLuint unit, GLint mip) const {
		glBindImageTexture(unit, m_ID, mip, GL_FALSE, 0, GL_READ_WRITE, m_format);
	}

	GLuint64 texture2D::getSamplerHandle() {
		// If sampler handle was not yet cached and made residend,
		// cache it and make it resident.
		if (!m_samplerHandle) {
			m_samplerHandle = glGetTextureHandleARB(m_ID);
			glMakeTextureHandleResidentARB(m_samplerHandle);
		}

		// return the cached handle
		return m_samplerHandle;
	}

	GLuint64 texture2D::getImageHandle(GLint mip) {
		// If sampler handle was not yet cached and made residend,
		// cache it and make it resident.
		if (!m_imageHandle) {
			m_imageHandle = glGetImageHandleARB(m_ID, mip, GL_FALSE, 0, m_format);
			glMakeImageHandleResidentARB(m_imageHandle, GL_READ_WRITE);
		}

		// return the cached handle
		return m_imageHandle;
	}
}
