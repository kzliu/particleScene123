#include "FramebufferObject.h"

FramebufferObject::FramebufferObject(int width, int height)
    : m_ID(0),
      m_p0_textureID(0),
      m_p1_textureID(0),
      m_v0_textureID(0),
      m_v1_textureID(0),
      m_width(width),
      m_height(height)
{
    // Create the FBO
    glGenFramebuffers(1, &m_ID);

    // Initializing the position and velocity textures
    createTexture(m_p0_textureID);
    createTexture(m_p1_textureID);
    createTexture(m_v0_textureID);
    createTexture(m_v1_textureID);

    // depth buffer stuff goes here if we do it

    // Unbinding
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void FramebufferObject::createTexture(const GLuint &textureID)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void FramebufferObject::setTextureImage(const GLuint &textureID, QImage image)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    if(image == NULL)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void FramebufferObject::bind()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_ID);
    glViewport(0, 0, m_width, m_height);
}

void FramebufferObject::swapTextures()
{
    GLuint temp = m_p0_textureID;
    m_p0_textureID = m_p1_textureID;
    m_p1_textureID = temp;

    temp = m_v0_textureID;
    m_v0_textureID = m_v1_textureID;
    m_v1_textureID = temp;
}
