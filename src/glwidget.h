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

    /** This creates a texture */
    void createTexture(const GLuint &textureID);

    /** This sets the content of a texture */
    void setTextureImage(const GLuint &textureID, QImage image);

    /** This swaps the 'front' and 'back' textures */
    void swapTextures();

private:
    std::unique_ptr<OpenGLShape> m_square;

    GLuint m_solidProgramID;
    GLuint m_gradientProgramID;
    GLuint m_textureProgramID;

    GLuint m_textureID;

    GLuint m_p0_textureID;
    GLuint m_p1_textureID;

    GLuint m_v0_textureID;
    GLuint m_v1_textureID;
};

#endif // GLWIDGET_H
