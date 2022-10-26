#include "Texture.h"

#include <glad/glad.h>
#include <engine/core.h>

namespace Amba {

	Texture::Texture() {}

	Texture::Texture(const std::string& fileName)
		:m_TexType(0), m_LocalBuffer(nullptr),
		m_Width(0), m_Height(0), m_BPP(0)
	{
		m_Directory = fileName.substr(0, fileName.find_last_of('/') + 1);
		m_FilePath = fileName.substr(fileName.find_last_of('/') + 1);
	}

	Texture::Texture(const std::string& dir, const std::string& path, unsigned int type)
		:m_RendererID(0), m_Directory(dir), m_FilePath(path), m_TexType(type), m_LocalBuffer(nullptr),
		m_Width(0), m_Height(0), m_BPP(0)
	{
	}

	Texture::~Texture()
	{	
	}

	void Texture::Bind(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void Texture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::LoadTexture(bool flip)
	{
		stbi_set_flip_vertically_on_load(flip);
		m_LocalBuffer = stbi_load((m_Directory + m_FilePath).c_str(), &m_Width, &m_Height, &m_BPP, 4);

		AB_ASSERT(m_LocalBuffer, ("Texture not loaded at" + m_Directory + m_FilePath));

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT /*GL_CLAMP_TO_EDGE*/);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT /*GL_CLAMP_TO_EDGE */);

		GLenum colorMode = GL_RGBA;
		switch (m_BPP)
		{
		case 1:
			colorMode = GL_RED;
			break;
		case 3:
			colorMode = GL_RGB;
			break;
		case 4:
			colorMode = GL_RGBA;
			break;
		default:
			break;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, colorMode, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
		Unbind();

		if (m_LocalBuffer)
			stbi_image_free(m_LocalBuffer);
	}
	void Texture::Cleanup()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	// temp
	unsigned int LoadCubemap(std::vector<std::string> faces)
	{
		stbi_set_flip_vertically_on_load(false);
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, nrChannels;

		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

			GLenum colorMode = GL_RGBA;
			switch (nrChannels)
			{
			case 1:
				colorMode = GL_RED;
				break;
			case 3:
				colorMode = GL_RGB;
				break;
			case 4:
				colorMode = GL_RGBA;
				break;
			default:
				break;
			}

			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, colorMode, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
				stbi_image_free(data);
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT /*GL_CLAMP_TO_EDGE*/);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT /*GL_CLAMP_TO_EDGE */);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT /*GL_CLAMP_TO_EDGE */);

		return textureID;
	}

}
