#ifndef FRAMEBUFFEROBJECT_H
#define FRAMEBUFFEROBJECT_H

#include "GL/glew.h"

class FramebufferObject
{
public:
    FramebufferObject(int width, int height);

    /** Binding the FBO */
    void bind();

    /** Unbinding the FBO */
    void unbind();

    /** Attaching the active color texture*/
    void attach(GLuint textureID);


private:
    GLuint m_ID;

    int m_width, m_height;

    GLuint m_depthBufferID;
};

#endif // FRAMEBUFFEROBJECT_H
