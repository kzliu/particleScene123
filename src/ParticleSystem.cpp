#include "ParticleSystem.h"
#include "FramebufferObject.h"

ParticleSystem::ParticleSystem()
    : m_p0_textureID(0),
      m_p1_textureID(0),
      m_v0_textureID(0),
      m_v1_textureID(0),
      m_particle_texture_width(50),
      m_particle_texture_height(50)
{
    // Initializing the position and velocity textures
    createTexture(m_p0_textureID);
    createTexture(m_p1_textureID);
    createTexture(m_v0_textureID);
    createTexture(m_v1_textureID);
}

void ParticleSystem::createTexture(GLuint &textureID)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_particle_texture_width, m_particle_texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void ParticleSystem::swapTextures()
{
    GLuint temp = m_p0_textureID;
    m_p0_textureID = m_p1_textureID;
    m_p1_textureID = temp;

    temp = m_v0_textureID;
    m_v0_textureID = m_v1_textureID;
    m_v1_textureID = temp;
}

void ParticleSystem::setTextureImage(const GLuint &textureID, QImage image)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    if(image.isNull())
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ParticleSystem::initializePositionAndVelocity()
{

}

void ParticleSystem::update(FramebufferObject fbo, GLuint updateShaderProgram)
{
    // This disables Gl blending the computed fragment colors with the values in the color buffers
    glDisable(GL_BLEND);

    // We begin by updating the positions texture
    fbo.attach(m_p1_textureID);

    // We bind the position and velocity textures to specific locations to be read by the shader
    bindActiveTexture(m_p0_textureID, 0);
    bindActiveTexture(m_v0_textureID, 1);

    // We use the update shader program
    glUseProgram(updateShaderProgram);

    // TODO: We're meant to do something with the quad here

    // Here we pass the shader our position and velocity textures
    glUniform1i(glGetUniformLocation(updateShaderProgram, "position"), 0);
    glUniform1i(glGetUniformLocation(updateShaderProgram, "velocity"), 1);
    // Now we send it specific values
    glUniform1f(glGetUniformLocation(updateShaderProgram, "random"), (GLfloat)rand() * 2.f - 1.f);
    glUniform1i(glGetUniformLocation(updateShaderProgram, "derivative"), 0);

    // TODO: DRAW

    // Next we update the velocity texture
    fbo.attach(m_v1_textureID);

    // We send the shader specific values this time to calculate velocity
    glUniform1f(glGetUniformLocation(updateShaderProgram, "random"), (GLfloat)rand() * 2.f - 1.f);
    glUniform1i(glGetUniformLocation(updateShaderProgram, "derivative"), 1);

    // TODO: DRAW

    swapTextures();

    glUseProgram(0);
}

void ParticleSystem::bindActiveTexture(GLuint textureID, GLenum textureUnit)
{
    if(textureUnit > 0)
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
    }
    glBindTexture(GL_TEXTURE_2D, textureID);
}
