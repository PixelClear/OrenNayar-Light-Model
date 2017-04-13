#include "TeslaCompute.h"
#include<string>
#include<iostream>

Texture::Texture(GLenum TextureTarget, const std::string& FileName)
{
    m_textureTarget = TextureTarget;
    m_fileName      = FileName;
}


bool Texture::Load()
{
    
	bitmap = FreeImage_Load(FreeImage_GetFileType(m_fileName.c_str()),m_fileName.c_str());
	p_Image = FreeImage_ConvertTo32Bits(bitmap);

	int width = FreeImage_GetWidth(p_Image);
	int height = FreeImage_GetHeight(p_Image);

    glGenTextures(1, &m_textureObj);
    glBindTexture(m_textureTarget, m_textureObj);
	glTexImage2D(m_textureTarget, 0, GL_RGBA, width , height, 0, GL_RGBA, GL_UNSIGNED_BYTE,(void*)FreeImage_GetBits(p_Image));
    glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    
    glBindTexture(m_textureTarget, 0);
    
	FreeImage_Unload(p_Image);
    return true;
}

void Texture::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
}
