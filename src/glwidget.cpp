#include "glwidget.h"
#include "settings.h"

#include "cs123_lib/resourceloader.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr

#include "openglshape.h"

GLWidget::GLWidget(QGLFormat format, QWidget *parent)
    : QGLWidget(format, parent),
      m_square(nullptr),
      m_solidProgramID(0),
      m_gradientProgramID(0),
      m_textureProgramID(0),
      m_updateProgramID(0),
      m_FBO1(nullptr), m_FBO2(nullptr),
      m_textureID(0)
{ }

GLWidget::~GLWidget()
{
    // TODO: Clean up GPU memory.
    glDeleteTextures(1,&m_textureID);

}


void GLWidget::initializeGL()
{    
    ResourceLoader::initializeGlew();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Defines the color the screen will be cleared to.

    // Creates the three shader programs.
    m_solidProgramID = ResourceLoader::createShaderProgram(":/shaders/solid.vert", ":/shaders/solid.frag");
    m_gradientProgramID = ResourceLoader::createShaderProgram(":/shaders/gradient.vert", ":/shaders/gradient.frag");
    m_textureProgramID = ResourceLoader::createShaderProgram(":/shaders/texture.vert", ":/shaders/texture.frag");
    m_updateProgramID = ResourceLoader::createShaderProgram(":/shaders/texture.vert", ":/shaders/update.frag");

    // Smart pointer!
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

    // TODO (Task 6): Initialize texture map.
    QImage image(":/images/ostrich.jpg");
    glGenTextures(1,&m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLWidget::paintGL()
{
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (settings.shaderProgram) {
    case SOLID_SHADER_PROGRAM:
        GLuint m_colorTextureID;
        glGenTextures(1, &m_colorTextureID);
        glBindTexture(GL_TEXTURE_2D, m_colorTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width(), this->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        //       [Task 3] Create and attach a color texture (using m_colorTextureID).
        m_FBO1->attach(m_colorTextureID);
        m_FBO1->bind();
//        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//        glUseProgram(0);

//        glViewport(0,0,this->width(),this->height());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // TODO (Task 1): Draw the square using m_solidProgramID.

        glUseProgram(m_gradientProgramID);

        // TODO (Task 2): Set the uniform's value to a color other than white
//        glUniform3f(glGetUniformLocation(m_gradientProgramID, "color"), 0.9, 0.0, 0.1);
        m_square->draw();

        glBindTexture(GL_TEXTURE_2D, m_colorTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glUseProgram(0);

        glViewport(0,0,this->width(),this->height());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(m_textureProgramID);
//        glBindTexture(GL_TEXTURE_2D, m_textureID);
        m_square->draw();

//        glBindTexture(GL_TEXTURE_2D,0);
        glUseProgram(0);
        break;

    case GRADIENT_SHADER_PROGRAM:
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // TODO (Task 5): Draw the square using m_gradientProgramID.
        glUseProgram(m_gradientProgramID);
        m_square->draw();
        break;
    case TEXTURE_SHADER_PROGRAM:
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // TODO (Task 8): Draw the square using m_textureProgramID.
        glUseProgram(m_textureProgramID);
        glBindTexture(GL_TEXTURE_2D, m_textureID);
        m_square->draw();
        glBindTexture(GL_TEXTURE_2D,0);
        break;
    }
    glUseProgram(0);

}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    m_FBO1.reset(new FramebufferObject(w,h));
    m_FBO2.reset(new FramebufferObject(w,h));
}
