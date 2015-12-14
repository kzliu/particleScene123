#ifndef FBO_H
#define FBO_H

#include "GL/glew.h"

class FBO
{
public:
    FBO(int width, int height);
    ~FBO();

    void bind();

    GLuint getColorTextureID();

private:
    GLuint m_ID;

    GLuint m_colorTextureID;

    GLuint m_depthBufferID;

    int m_width, m_height;
};

#endif // FBO_H
