#include "FramebufferObject.h"

FramebufferObject::FramebufferObject(int width, int height)
    : m_ID(0),
      m_width(width),
      m_height(height),
      m_depthBufferID(0)
{
    // Create the FBO
    glGenFramebuffers(1, &m_ID);

    // depth buffer if needed
    glGenRenderbuffers(1,&m_depthBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,m_width, m_height);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferID);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);


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


