#pragma once

#include <iostream>
#include <STB/stb_image.h>

namespace Amba {

	class Texture
	{
	public:
        Texture();
        Texture(const std::string& fileName);
		Texture(const std::string& dir, const std::string& path, unsigned int type);
		~Texture();

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

        void LoadTexture(bool flip = true);
		
        inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }

        void Cleanup();

	private:
		unsigned int m_RendererID;
        std::string m_Directory;
		std::string m_FilePath;
        unsigned int m_TexType;
		unsigned char* m_LocalBuffer;
		int m_Width, m_Height, m_BPP;
	

    friend class Model;
    friend class Renderer;
    };

}