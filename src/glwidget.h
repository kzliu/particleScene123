#ifndef GLWIDGET_H
#define GLWIDGET_H
#include "GL/glew.h"
#ifdef __APPLE__
#include <glu.h>
#else
#include "GL/glu.h"
#endif
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr

#include <iostream>
#include <memory>
#include <QGLWidget>
#include <QTimer>

#include "FramebufferObject.h";

class OpenGLShape;

class ParticleSystem;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QGLFormat format, QWidget *parent = 0);
    ~GLWidget();

protected:
    /** This is called once, before any calls to paintGL. */
    void initializeGL();

    /** This is called every time the window needs to be repainted. */
    void paintGL();

    void resizeGL(int w, int h);

    bool loadOBJ(const char * path,  std::vector<GLfloat> &vertex_vector);

    glm::vec3 calculateTan(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec2 uv0, glm::vec2 uv1, glm::vec2 uv2);

protected slots:
    /** Repaints the canvas. Called 60 times per second by m_timer. */
    void tick();

private:
    std::unique_ptr<OpenGLShape> m_square;

    GLuint m_solidProgramID;
    GLuint m_gradientProgramID;
    GLuint m_textureProgramID;
    GLuint m_updateProgramID;
    GLuint m_drawProgramID;


    std::unique_ptr<FramebufferObject> m_FBO1;
    std::unique_ptr<FramebufferObject> m_FBO2;

    GLuint m_textureID;
    GLuint m_textureID_stone;
    GLuint m_noiseTexture;

    /** Timer calls tick() 60 times per second. */
    QTimer m_timer;
    float m_fps;

    /** Incremented on every call to paintGL. */
    int m_increment;

    std::unique_ptr<ParticleSystem> m_particles;
    std::vector<GLfloat> m_vertex_vector;
};

#endif // GLWIDGET_H
