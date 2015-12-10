#ifndef FRAMEBUFFEROBJECT_H
#define FRAMEBUFFEROBJECT_H

#include "GL/glew.h"

class FramebufferObject
{
public:
    FramebufferObject();
    ~FramebufferObject();

    /** This creates a texture */
    void createTexture(const GLuint &textureID);

    /** This sets the content of a texture */
    void setTextureImage(const GLuint &textureID, QImage image);

    /** This swaps the 'front' and 'back' textures */
    void swapTextures();

private:
    GLuint m_ID;
    GLuint m_p0_textureID;
    GLuint m_p1_textureID;

    GLuint m_v0_textureID;
    GLuint m_v1_textureID;
};

#endif // FRAMEBUFFEROBJECT_H
