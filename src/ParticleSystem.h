#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "GL/glew.h"
#include <QImage>
#include <memory>

#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr

class OpenGLShape;
class FramebufferObject;
//class QRgb;

class ParticleSystem
{
public:
    ParticleSystem(int texture_width, int texture_height, GLuint canvas_width, GLuint canvas_height, GLuint scale_p, GLuint scale_v, GLuint particle_size, QRgb particle_color);

    /** Encodes a value given the scale */
    glm::vec2 encode(GLuint value, GLuint scale);

    /** This creates a texture */
    void createTexture(GLuint &textureID);

    /** This sets the content of a texture */
    void setTextureImage(const GLuint &textureID, QImage image);

    /** This swaps the 'front' and 'back' textures */
    void swapTextures();

    /** Moves the particle system forward by one iteration */
    void update(GLuint &updateShaderProgram, std::unique_ptr<OpenGLShape>& quad);

    /** Draws the current state */
    void draw(const GLuint &drawShaderProgram, OpenGLShape points);

    /** Setting and binding the active texture */
    void bindActiveTexture(const GLuint &textureID, GLenum textureUnit);

    /** Initialize the position and velocity */
    void initializePositionAndVelocity();
    
    GLuint get_p0texture();

    GLuint get_v0texture();

    GLuint get_p0texture();

    GLuint get_v0texture();

    GLuint get_p1texture();

    GLuint get_v1texture();

private:
    GLuint m_p0_textureID;
    GLuint m_p1_textureID;

    GLuint m_v0_textureID;
    GLuint m_v1_textureID;

    GLuint m_particle_texture_width;
    GLuint m_particle_texture_height;

    GLuint m_canvas_width;
    GLuint m_canvas_height;

    GLuint m_scale_p;
    GLuint m_scale_v;

    GLuint m_particle_size;
    QRgb m_particle_color;

    std::unique_ptr<FramebufferObject> m_FBO1;
    std::unique_ptr<FramebufferObject> m_FBO2;

    std::unique_ptr<OpenGLShape> m_square;

};

#endif // PARTICLESYSTEM_H
