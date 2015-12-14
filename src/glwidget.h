#ifndef GLWIDGET_H
#define GLWIDGET_H
#include "GL/glew.h"
#ifdef __APPLE__
#include <glu.h>
#else
#include "GL/glu.h"
#endif

#include <memory>
#include <QGLWidget>
#include <QTimer>
#include "FramebufferObject.h"

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

    /** Timer calls tick() 60 times per second. */
    QTimer m_timer;
    float m_fps;

    /** Incremented on every call to paintGL. */
    int m_increment;

    std::unique_ptr<ParticleSystem> m_particles;
};

#endif // GLWIDGET_H
