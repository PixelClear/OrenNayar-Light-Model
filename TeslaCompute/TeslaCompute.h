#ifndef _pgl_tesla
#define _pgl_tesla

#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include <GL/glew.h>
#include<GL/freeglut.h>
#include<glm/glm.hpp>
#include<glm/ext.hpp>
#include "FreeImage.h"

class Texture
{
public:
    Texture(GLenum TextureTarget, const std::string& FileName);

    bool Load();

    void Bind(GLenum TextureUnit);

private:
    std::string m_fileName;
    GLenum m_textureTarget;
    GLuint m_textureObj;
    FIBITMAP *bitmap;
    FIBITMAP *p_Image;
};

#endif