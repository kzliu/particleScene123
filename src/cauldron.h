#ifndef CAULDRON_H
#define CAULDRON_H


#include "GL/glew.h"
#include <QGLWidget>
#include <QTimer>

#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr

#include <memory>  // std::unique_ptr

class OpenGLShape;
class FBO;

class cauldron
{
public:
    cauldron(int width, int height, glm::mat4 &view, glm::mat4 &projection, glm::mat4 &model, std::vector<GLfloat> &vertex_vector, GLuint &normalID, GLuint &textureID) ;
    ~cauldron();

    void update(int width, int height, glm::mat4 view, glm::mat4 projection, glm::mat4 model);
    void render();

protected:
    void initialize();

    GLfloat noiseLerp(GLfloat a, GLfloat b, GLfloat f);
    void genNoiseTexture();


private:

    int m_width;
    int m_height;

    GLuint m_normalProgram;
    GLuint m_depthProgram;
    GLuint m_ambientProgram;
    GLuint m_myPhongProgram;
    GLuint m_positionProgram;
    GLuint m_stoneUVProgram;

    GLuint m_textureID;
    GLuint m_textureID_stone;
    GLuint m_noiseTexture;

    std::vector<GLfloat> m_vertex_vector;

    std::unique_ptr<OpenGLShape> m_quad;
    std::unique_ptr<OpenGLShape> m_cauldron;

    std::unique_ptr<FBO> m_positionFBO;
    std::unique_ptr<FBO> m_normalFBO;
    std::unique_ptr<FBO> m_depthFBO;
    std::unique_ptr<FBO> m_ambientFBO;
    std::unique_ptr<FBO> m_stoneUVFBO;
    std::unique_ptr<FBO> m_FBO1;


    glm::mat4 m_view, m_projection, m_model;

    std::vector<glm::vec3> m_kernel;
    std::vector<glm::vec3> m_noise;


};

#endif // CAULDRON_H
