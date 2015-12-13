#include "ParticleSystem.h"
#include "openglshape.h"
#include "FramebufferObject.h"
#include <iostream>
#include <memory>


ParticleSystem::ParticleSystem(int texture_width, int texture_height, GLuint canvas_width, GLuint canvas_height, GLuint scale_p, GLuint scale_v, GLuint particle_size, QRgb particle_color)
    : m_p0_textureID(0),
      m_p1_textureID(0),
      m_v0_textureID(0),
      m_v1_textureID(0),
      m_particle_texture_width(texture_width),
      m_particle_texture_height(texture_height),
      m_canvas_width(canvas_width),
      m_canvas_height(canvas_height),
      m_scale_p(scale_p),
      m_scale_v(scale_v),
      m_particle_size(particle_size),
      m_particle_color(particle_color),
      m_FBO1(nullptr),
      m_FBO2(nullptr)
{
    // Initializing the position and velocity textures
    createTexture(m_p0_textureID);
    createTexture(m_p1_textureID);
    createTexture(m_v0_textureID);
    createTexture(m_v1_textureID);

    initializePositionAndVelocity();
    m_FBO1.reset(new FramebufferObject(texture_width,texture_height));
    m_FBO2.reset(new FramebufferObject(texture_width,texture_height));

    m_square.reset(new OpenGLShape());

    // TODO (Task 1): Initialize m_square.
    // TODO (Task 3): Interleave positions and colors in the array used to intialize m_square
    // TODO (Task 7): Interleave UV-coordinates along with positions and colors in your VBO
    GLfloat data[32] = {-0.5f, 0.5f, 0.f,
                        1.f, 0.f, 0.f,
                        0.f, 1.f,
                        -0.5f, -0.5f, 0.f,
                        0.f, 1.f, 0.f,
                        0.f, 0.f,
                        0.5f, 0.5f, 0.f,
                        1.f, 1.f, 0.f,
                        1.f, 1.f,
                        0.5f, -0.5f, 0.f,
                        0.f, 0.f, 1.f,
                        1.f, 0.f };
    m_square->setVertexData(data, 32*sizeof(GLfloat), GL_TRIANGLE_STRIP, 4);
    m_square->setAttribute(0, 3, GL_FLOAT, GL_FALSE, 32, 0);
    m_square->setAttribute(1, 3, GL_FLOAT, GL_FALSE, 32, 12);
    m_square->setAttribute(2, 2, GL_FLOAT, GL_FALSE, 32, 24);
}

glm::vec2 ParticleSystem::encode(GLuint value, GLuint scale)
{
    GLuint base = 255;
    value = value * scale + base * base / 2;

    glm::vec2 pair = glm::vec2(glm::floor((GLfloat)(value % base) / base * 255), glm::floor((GLfloat)glm::floor((GLfloat)value / base) / base * 255));

    return pair;
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


//TODO: initialize indices buffer


void ParticleSystem::initializePositionAndVelocity()
{
    QImage position_texture(m_particle_texture_width, m_particle_texture_height, QImage::Format_ARGB32);
    position_texture.fill(Qt::white);

    QImage velocity_texture(m_particle_texture_width, m_particle_texture_height, QImage::Format_ARGB32);
    velocity_texture.fill(Qt::white);


    for (unsigned int y = 0; y < m_particle_texture_height; y++) {
        for (unsigned int x = 0; x < m_particle_texture_width; x++){
//            int index = y * m_particle_texture_width + x;
            // TODO: check this out in debugging
            glm::vec2 p_x = encode(rand() % m_canvas_width, m_scale_p);
            glm::vec2 p_y = encode(0.5 * m_canvas_height, m_scale_p);
            glm::vec2 v_x = encode(rand() % 2 - 1, m_scale_v);
            glm::vec2 v_y = encode(-1 * (rand() % 2), m_scale_v); //why is this negative?

//            std::cout << p_x.x << std::endl;

            QRgb p_c = qRgba(p_x[0], p_x[1], p_y[0], p_y[1]);
            QRgb v_c = qRgba(v_x[0], v_x[1], v_y[0], v_y[1]);

            position_texture.setPixel(x,y, p_c);

            velocity_texture.setPixel(x,y, v_c);
        }
    }

    setTextureImage(m_p0_textureID, position_texture);
    setTextureImage(m_v0_textureID, velocity_texture);

}

void ParticleSystem::update(GLuint &updateShaderProgram, std::unique_ptr<OpenGLShape>& quad)
{
    // This disables Gl blending the computed fragment colors with the values in the color buffers
    glDisable(GL_BLEND);

//    m_FBO1->attach(m_p1_textureID);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glUseProgram(0);

    glViewport(0,0,m_canvas_width,m_canvas_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // We begin by updating the positions texture

    // We use the update shader program
    glUseProgram(updateShaderProgram);

    // Here we pass the shader our position and velocity textures
    glUniform1i(glGetUniformLocation(updateShaderProgram, "position"), 0);
    glUniform1i(glGetUniformLocation(updateShaderProgram, "velocity"), 1);



    // We bind the position and velocity textures to specific locations to be read by the shader
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, m_p0_textureID);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_v0_textureID);


    // Now we send it specific values
    glUniform1f(glGetUniformLocation(updateShaderProgram, "random"), rand() % 2 - 1.f);
    glUniform1i(glGetUniformLocation(updateShaderProgram, "derivative"), 0);
    glUniform1i(glGetUniformLocation(updateShaderProgram, "pscale"), m_scale_p);
    glUniform1i(glGetUniformLocation(updateShaderProgram, "vscale"), m_scale_v);

    // Now, we draw
//    m_square->draw();

//    glBindTexture(GL_TEXTURE_2D, m_p1_textureID);

//    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//    glUseProgram(0);

//    glViewport(0,0,m_canvas_width,m_canvas_height);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    glUseProgram(updateShaderProgram);
//    m_square->draw();

//    // Next we update the velocity texture
//    fbo.attach(m_v1_textureID);

//    // We send the shader specific values this time to calculate velocity
//    glUniform1f(glGetUniformLocation(updateShaderProgram, "random"), rand() % 2 - 1.f);
//    glUniform1i(glGetUniformLocation(updateShaderProgram, "derivative"), 1);

//    // We draw again
//    quad.draw();

//    swapTextures();

    glUseProgram(0);
}

void ParticleSystem::draw(const GLuint &drawShaderProgram, OpenGLShape points)
{
    // This enables Gl blending the computed fragment colors with the values in the color buffers
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Blinding the default frame buffer (the screen)
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glViewport(0, 0, m_canvas_width, m_canvas_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // We bind the position and velocity textures to specific locations to be read by the shader
    bindActiveTexture(m_p0_textureID, 0);
    bindActiveTexture(m_v0_textureID, 1);

    // We use the draw shader program
    glUseProgram(drawShaderProgram);

    // Here we pass the shader our position and velocity textures
    glUniform1i(glGetUniformLocation(drawShaderProgram, "positions"), 0);
    glUniform1i(glGetUniformLocation(drawShaderProgram, "velocities"), 1);

    // Now we send it specific values
    glUniform2i(glGetUniformLocation(drawShaderProgram, "statedimensions"), m_particle_texture_width, m_particle_texture_height);
    glUniform2i(glGetUniformLocation(drawShaderProgram, "worlddimensions"), m_canvas_width, m_canvas_height);
    glUniform1f(glGetUniformLocation(drawShaderProgram, "pscale"), m_scale_p);
    glUniform1f(glGetUniformLocation(drawShaderProgram, "vscale"), m_scale_v);
//    glUniform1f(glGetUniformLocation(drawShaderProgram, "particlesize"), m_particle_size);
    glUniform4i(glGetUniformLocation(drawShaderProgram, "particlecolor"), qRed(m_particle_color), qGreen(m_particle_color), qBlue(m_particle_color), qAlpha(m_particle_color));

    // TODO:And now we draw (using GL_POINTS)
    points.draw();

    glUseProgram(0);
}

void ParticleSystem::bindActiveTexture(const GLuint &textureID, GLenum textureUnit)
{
    if(textureUnit > 0)
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
    }
    glBindTexture(GL_TEXTURE_2D, textureID);
}

GLuint ParticleSystem::get_p0texture(){
    return m_p0_textureID;
}

GLuint ParticleSystem::get_v0texture(){
    return m_v0_textureID;
}


GLuint ParticleSystem::get_p1texture(){
    return m_p1_textureID;
}

GLuint ParticleSystem::get_v1texture(){
    return m_v1_textureID;
}

