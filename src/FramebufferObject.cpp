#include "FramebufferObject.h"

FramebufferObject::FramebufferObject(int width, int height)
    : m_ID(0),
      m_width(width),
      m_height(height)
{
    // Create the FBO
    glGenFramebuffers(1, &m_ID);

    // Unbinding
    unbind();
}

void FramebufferObject::attach(GLuint textureID)
{
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
    unbind();
}

void FramebufferObject::bind()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_ID);
    glViewport(0, 0, m_width, m_height);
}

void FramebufferObject::unbind()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glViewport(0, 0, m_width, m_height);
}
