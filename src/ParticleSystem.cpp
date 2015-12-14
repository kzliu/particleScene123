#include "ParticleSystem.h"
#include "openglshape.h"
#include "FramebufferObject.h"
#include <iostream>
#include <memory>


ParticleSystem::ParticleSystem(int texture_width, int texture_height, GLuint canvas_width, GLuint canvas_height, GLuint scale_p, GLuint scale_v, GLfloat particle_size, QRgb particle_color)
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
    // We create the velocity textures
    createTexture(m_p0_textureID);
    createTexture(m_p1_textureID);
    createTexture(m_v0_textureID);
    createTexture(m_v1_textureID);

    // Now we initialize the textures with specific values
    initializePositionAndVelocity();

    m_FBO1.reset(new FramebufferObject(m_particle_texture_width,m_particle_texture_height));
    m_FBO2.reset(new FramebufferObject(m_particle_texture_width,m_particle_texture_height));

    // Now we populate the quads we use in drawing and updating
    m_square.reset(new OpenGLShape());
    m_indices.reset(new OpenGLShape());

    GLfloat square_data[32] = {-0.5f, 0.5f, 0.f,
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
    m_square->setVertexData(square_data, 32*sizeof(GLfloat), GL_TRIANGLE_STRIP, 4);
    m_square->setAttribute(0, 3, GL_FLOAT, GL_FALSE, 32, 0);
    m_square->setAttribute(1, 3, GL_FLOAT, GL_FALSE, 32, 12);
    m_square->setAttribute(2, 2, GL_FLOAT, GL_FALSE, 32, 24);

    int indices_size = m_particle_texture_width * m_particle_texture_height * 2;
    GLfloat indices_data[indices_size] = {0};
    for(int y = 0; y < m_particle_texture_height; y++){
        for(int x = 0; x < m_particle_texture_width; x++){
            int i = y * m_particle_texture_width * 2 + x * 2;
            indices_data[i] = (GLfloat) x;
            indices_data[i + 1] = (GLfloat) y;
        }
    }

    m_indices->setVertexData(indices_data, indices_size*sizeof(GLfloat), GL_POINTS, indices_size / 2);
    m_indices->setAttribute(0, 2, GL_FLOAT, GL_FALSE, 8, 0);
}

glm::vec2 ParticleSystem::encode(GLuint value, GLuint scale)
{
    GLuint base = 255;
    value = value * scale + base * base / 2;

    glm::vec2 pair = glm::vec2(glm::floor((GLfloat)(value % base) / base * 255), glm::floor((GLfloat)glm::floor((GLfloat)(value / base)) / base * 255));

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
            glm::vec2 p_x = encode(rand() % (int)floor(m_canvas_width/3) + floor(m_canvas_width/3) , m_scale_p);
            glm::vec2 p_y = encode(rand() % (int)floor(m_canvas_width/3) + floor(m_canvas_height*2/3), m_scale_p);
            glm::vec2 v_x = encode(rand() % 2 - 1, m_scale_v);
            glm::vec2 v_y = encode((rand() % 2), m_scale_v); //why is this negative?

            QRgb p_c = qRgba(p_x[0], p_x[1], p_y[0], p_y[1]);
            QRgb v_c = qRgba(v_x[0], v_x[1], v_y[0], v_y[1]);

            position_texture.setPixel(x,y, p_c);

            velocity_texture.setPixel(x,y, v_c);
        }
    }

    setTextureImage(m_p0_textureID, position_texture);
    setTextureImage(m_v0_textureID, velocity_texture);
}

void ParticleSystem::update(GLuint &shaderProgramID)
{
    m_FBO2.reset(new FramebufferObject(m_particle_texture_width,m_particle_texture_height));
    // This disables Gl blending the computed fragment colors with the values in the color buffers
    glDisable(GL_BLEND);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_FBO2->attach(m_p1_textureID);

    glUseProgram(shaderProgramID);

    bindActiveTexture(m_p0_textureID, 0);
    bindActiveTexture(m_v0_textureID, 1);

    glViewport(0,0,m_particle_texture_width,m_particle_texture_height);

    glUniform1i(glGetUniformLocation(shaderProgramID, "position"), 0);
    glUniform1i(glGetUniformLocation(shaderProgramID, "velocity"), 1);

    glUniform1f(glGetUniformLocation(shaderProgramID, "pscale"), m_scale_p);
    glUniform1f(glGetUniformLocation(shaderProgramID, "vscale"), m_scale_v);

    glUniform2f(glGetUniformLocation(shaderProgramID, "worlddimensions"), m_canvas_width, m_canvas_height);
    glUniform1f(glGetUniformLocation(shaderProgramID, "random"), rand() % 2 - 1.f);
    glUniform1i(glGetUniformLocation(shaderProgramID, "derivative"), 0);

    m_square->draw();

//    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    m_FBO2->attach(m_v1_textureID);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLfloat m_rand = (GLfloat)rand() / (GLfloat)RAND_MAX;

    glUniform1f(glGetUniformLocation(shaderProgramID, "random"), m_rand * 2.f - 1.f);
    glUniform1i(glGetUniformLocation(shaderProgramID, "derivative"), 1);

    // Draw into the frame buffer
    m_square->draw();

    // Swapping the updated textures
    swapTextures();

    glBindTexture(GL_TEXTURE_2D,0);
    glUseProgram(0);
    m_FBO2->unbind();
}

void ParticleSystem::draw(GLuint &drawShaderProgram)
{
    // This enables Gl blending the computed fragment colors with the values in the color buffers
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_PROGRAM_POINT_SIZE);
    // Blinding the default frame buffer (the screen)
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

//    glClearColor(0, 0, 0, 1);
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
    glUniform2f(glGetUniformLocation(drawShaderProgram, "statedimensions"), m_particle_texture_width, m_particle_texture_height);
    glUniform2f(glGetUniformLocation(drawShaderProgram, "worlddimensions"), m_canvas_width, m_canvas_height);
    glUniform1f(glGetUniformLocation(drawShaderProgram, "pscale"), m_scale_p);
    glUniform1f(glGetUniformLocation(drawShaderProgram, "vscale"), m_scale_v);
    glUniform1f(glGetUniformLocation(drawShaderProgram, "particlesize"), m_particle_size);
    glUniform4f(glGetUniformLocation(drawShaderProgram, "particlecolor"), (GLfloat)qRed(m_particle_color)/255.f, (GLfloat)qGreen(m_particle_color)/255.f, (GLfloat)qBlue(m_particle_color)/255.f, (GLfloat)qAlpha(m_particle_color)/255.f);

    // Now we draw
    glViewport(0, 0, m_canvas_width, m_canvas_height);
    m_indices->draw();

//    glBindTexture(GL_TEXTURE_2D,0);

    glUseProgram(0);
}

void ParticleSystem::bindActiveTexture(const GLuint &textureID, GLenum textureUnit)
{
    if(textureUnit > 0)
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
    }
    else {
        glActiveTexture(GL_TEXTURE0);
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

