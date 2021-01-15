/*
 texture.hpp
 Breakout
 Last updated on September 30, 2020
 
 Declaration of the Texture2D class
 The Texture2D class encapsulates properties of OpenGL 2D textures. It facilitates creations and usages of textures in the game.
 */

#ifndef texture_hpp
#define texture_hpp

#include <glad/glad.h>

class Texture2D {
public:
    unsigned int ID;
    unsigned int Width;
    unsigned int Height;
    unsigned int Image_Format;
    unsigned int Internal_Format;
    unsigned int Wrap_S;
    unsigned int Wrap_T;
    unsigned int Mag_Filter;
    unsigned int Min_Filter;
    
    Texture2D();
    
    void Generate(unsigned int width, unsigned int height, unsigned char *data);
    void Bind() const;
};

#endif /* texture_hpp */
