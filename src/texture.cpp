#include "glrhi/texture.hpp"

glrhi::texture::texture() {
    glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);

	glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

glrhi::texture::~texture() {
    glDeleteTextures(1, &m_ID);
}

void glrhi::texture::loadEmpty(unsigned int width, unsigned int height, GLenum internalFormat) const {
    glTextureStorage2D(m_ID, 1, internalFormat, width, height);
}

void glrhi::texture::loadData(unsigned int width, unsigned int height, GLenum internalFormat, GLenum dataFormat, GLenum type, const void* data) const {
    loadEmpty(width, height, internalFormat);
	glTextureSubImage2D(m_ID, 0, 0, 0, width, height, dataFormat, type, data);  
}

void glrhi::texture::loadFile(const char* path, GLenum type, GLenum internalFormat) const {
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(path, &widthImg, &heightImg, &numColCh, 0);
    
    GLenum lookup[5] = {0, GL_RED, GL_RG, GL_RGB, GL_RGBA}; 

	loadData(widthImg, heightImg, internalFormat, lookup[numColCh], type, bytes);

	stbi_image_free(bytes);   
}

void glrhi::texture::bind(GLuint textureUnit, glrhi::shader& shader, const char* textureUniform) const {
	glBindTextureUnit(textureUnit, m_ID);
	shader.setInt(textureUniform, textureUnit);
}