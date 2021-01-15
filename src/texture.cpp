/*
 texture.cpp
 Breakout
 Last updated on September 30, 2020
 
 Implementation of the Texture2D class
 The Texture2D class encapsulates properties of OpenGL 2D textures. It facilitates creations and usages of textures in the game.
 */

#include "texture.hpp"

Texture2D::Texture2D() : Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Mag_Filter(GL_LINEAR), Min_Filter(GL_LINEAR) {
    glGenTextures(1, &ID);
}

void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char *data) {
    Width = width;
    Height = height;
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, Internal_Format, width, height, 0, Image_Format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Mag_Filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Min_Filter);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const {
    glBindTexture(GL_TEXTURE_2D, ID);
}
