#ifndef FRAMEBUFFEROBJECT_H
#define FRAMEBUFFEROBJECT_H

#include "GL/glew.h"
#include <QImage>

class FramebufferObject
{
public:
    FramebufferObject(int width, int height);
    ~FramebufferObject();

    /** This creates a texture */
    void createTexture(const GLuint &textureID);

    /** This sets the content of a texture */
    void setTextureImage(const GLuint &textureID, QImage image);

    /** This swaps the 'front' and 'back' textures */
    void swapTextures();

    /** Binding the FBO */
    void bind();

private:
    GLuint m_ID;
    GLuint m_p0_textureID;
    GLuint m_p1_textureID;

    GLuint m_v0_textureID;
    GLuint m_v1_textureID;

    int m_width, m_height;
};

#endif // FRAMEBUFFEROBJECT_H
