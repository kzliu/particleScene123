#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "GL/glew.h"
#include <QImage>

class FramebufferObject;

class ParticleSystem
{
public:
    ParticleSystem();

    /** This creates a texture */
    void createTexture(const GLuint &textureID);

    /** This sets the content of a texture */
    void setTextureImage(const GLuint &textureID, QImage image);

    /** This swaps the 'front' and 'back' textures */
    void swapTextures();

    /** Moves the particle system forward by one iteration */
    void update(FramebufferObject fbo, GLuint updateShaderProgram);

    /** Setting and binding the active texture */
    void bindActiveTexture(GLuint textureID, GLenum textureUnit);

    /** Initialize the position and velocity */
    void initializePositionAndVelocity();

private:
    GLuint m_p0_textureID;
    GLuint m_p1_textureID;

    GLuint m_v0_textureID;
    GLuint m_v1_textureID;

    GLuint m_particle_texture_width;
    GLuint m_particle_texture_height;
};

#endif // PARTICLESYSTEM_H
