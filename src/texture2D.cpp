#include "glrhi/core/texture2D.hpp"


namespace glrhi {

	texture2D::texture2D(GLsizei width, 
								GLsizei height, 
								GLenum internalFormat, 
								GLsizei mips) : m_width(width), m_height(height), m_format(internalFormat) {

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);

		if (mips > 1)
			glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureStorage2D(m_ID, mips, internalFormat, width, height);

	}

	texture2D::texture2D(const char* file, GLenum internalFormat, GLsizei mips) {

		// Texture creation
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);

		// Texture options
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Image properties
		int widthImg, heightImg, numColCh;
		GLenum dataType = GL_UNSIGNED_BYTE;
		bool hdr = false;
		void* data = nullptr;

		// check image properties
		if (stbi_is_hdr(file)) {
			hdr = true;
			dataType = GL_FLOAT;
		}

		// Loading image into memory
		stbi_set_flip_vertically_on_load(true);
		if (!hdr)
			data = stbi_load(file, &widthImg, &heightImg, &numColCh, 4);
		else
			data = stbi_loadf(file, &widthImg, &heightImg, &numColCh, 4);

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

		stbi_image_free(data);  
	}

	texture2D::~texture2D() {
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

	void texture2D::bind(GLuint textureUnit, glrhi::shader& shader, const char* textureUniform) const {
		glBindTextureUnit(textureUnit, m_ID);
		shader.setInt(textureUniform, textureUnit);
	}

	void texture2D::bindImage(GLuint unit, GLint mip, GLenum access) const {
		glBindImageTexture(unit, m_ID, mip, GL_FALSE, 0, access, m_format);
	}
}
