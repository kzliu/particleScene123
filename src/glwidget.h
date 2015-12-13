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

private:
    std::unique_ptr<OpenGLShape> m_square;

    GLuint m_solidProgramID;
    GLuint m_gradientProgramID;
    GLuint m_textureProgramID;
    GLuint m_updateProgramID;


    std::unique_ptr<FramebufferObject> m_FBO1;
    std::unique_ptr<FramebufferObject> m_FBO2;

    GLuint m_textureID;
};

#endif // GLWIDGET_H
