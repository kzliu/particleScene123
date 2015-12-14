#include "FBO.h"

FBO::FBO(int width, int height)
    : m_ID(0),
      m_colorTextureID(0),
      m_depthBufferID(0),
      m_width(width),
      m_height(height)
{
    // TODO: [Task 2] Create an FBO (using m_ID).
    glGenFramebuffers(1,&m_ID);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_ID);   //bind the FBO

    glGenTextures(1, &m_colorTextureID);
    glBindTexture(GL_TEXTURE_2D, m_colorTextureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    //       [Task 3] Create and attach a color texture (using m_colorTextureID).
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTextureID,0);
    glBindTexture(GL_TEXTURE_2D, 0);

    //       [Task 8] Create and attach a depth buffer (using m_depthBufferID).

    glGenTextures(1, &m_depthBufferID);
    glBindTexture(GL_TEXTURE_2D, m_depthBufferID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);    //set parameters of depth buffer
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_ID);                                                   // bind our FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBufferID, 0); // attach our depth buffer

    glBindTexture(GL_TEXTURE_2D, 0);                // unbind texture
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);      // unbind FBO

//    glGenRenderbuffers(1,&m_depthBufferID);
//    glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferID);
//    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,m_width, m_height);
//    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferID);
//    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

}

FBO::~FBO()
{
    // TODO: [Task 9] Delete all OpenGL objects that were generated in the constructor.
    glDeleteTextures(1, &m_colorTextureID);
    glDeleteTextures(1, &m_depthBufferID);
    glDeleteFramebuffers(1, &m_ID);

}

void FBO::bind()
{
    // TODO: [Task 4] 1) Bind the framebuffer.
    //                2) Use glViewport to make sure the texture is rendered with the correct dimensions.
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_ID);
    glViewport(0.f,0.f,m_width,m_height);
//    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); do we need to do this?
}

GLuint FBO::getColorTextureID()
{
    return m_colorTextureID;
}
