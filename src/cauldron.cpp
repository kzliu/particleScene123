#include "cauldron.h"

#include "cs123_lib/resourceloader.h"
#include <iostream>

#include "openglshape.h"
#include "framebufferobject.h"
#include "sphere.h"
#include <QDebug>

cauldron::cauldron(int width, int height, glm::mat4 &view, glm::mat4 &projection, glm::mat4 &model, std::vector<GLfloat> &vertex_vector, GLuint &normalID, GLuint &textureID) :
    m_width(width), m_height(height),
    m_normalProgram(0), m_depthProgram(0), m_ambientProgram(0), m_myPhongProgram(0), m_positionProgram(0), m_stoneUVProgram(0),
    m_textureID(normalID), m_textureID_stone(textureID), m_noiseTexture(0),
    m_quad(nullptr), m_cauldron(nullptr),
    m_positionFBO(nullptr), m_normalFBO(nullptr), m_depthFBO(nullptr), m_ambientFBO(nullptr), m_stoneUVFBO(nullptr),
    m_view(view), m_projection(projection), m_model(model),
    m_vertex_vector(vertex_vector)
{

    initialize();
}

cauldron::~cauldron() {

}

void cauldron::initialize(){
    ResourceLoader::initializeGlew();
    //shaders
    m_normalProgram = ResourceLoader::createShaderProgram(
                ":/shaders/normal.vert", ":/shaders/normal.frag");
    m_depthProgram = ResourceLoader::createShaderProgram(
                ":/shaders/depth.vert", ":/shaders/depth.frag");
    m_ambientProgram = ResourceLoader::createShaderProgram(
                ":/shaders/quad.vert", ":/shaders/ambient.frag");
    m_myPhongProgram = ResourceLoader::createShaderProgram(
                ":/shaders/quad.vert", ":/shaders/myPhong.frag");
    m_positionProgram = ResourceLoader::createShaderProgram(
                ":/shaders/position.vert", ":/shaders/position.frag");
    m_stoneUVProgram = ResourceLoader::createShaderProgram(
                ":/shaders/color.vert", ":/shaders/color.frag");

//    int numFloats = vertex_vector.size()*3 + uvs.size()*2;
    GLfloat vertex_data[m_vertex_vector.size()];
    std::copy( m_vertex_vector.begin(), m_vertex_vector.end(), vertex_data );

   //end new stuff

//    qDebug() << vertex_vector.size()/11.0;

    int num_vert = m_vertex_vector.size()/11;
    m_cauldron.reset(new OpenGLShape());
    m_cauldron->setVertexData(vertex_data, sizeof(vertex_data), GL_TRIANGLES, num_vert);

    m_cauldron->setAttribute(0, 3, GL_FLOAT, GL_FALSE, 44, 0);
    m_cauldron->setAttribute(1, 2, GL_FLOAT, GL_FALSE, 44, 12);
    m_cauldron->setAttribute(2, 3, GL_FLOAT, GL_TRUE, 44, 20);
    m_cauldron->setAttribute(3, 3, GL_FLOAT, GL_FALSE, 44, 32);


    // TODO: [Task 6] Use m_quad to initialize a full-screen quad with positions and UV-coordinates.
    GLfloat quadData[20] = {-1.f, 1.f, 0.f,
                            0.f,1.f,
                            -1.f,-1.f, 0.f,
                            0.f,0.f,
                            1.f,1.f,0.f,
                            1.f,1.f,
                            1.f,-1.f,0.f,
                            1.f,0.f};

    m_quad.reset(new OpenGLShape());
    m_quad->setVertexData(quadData, sizeof(quadData), GL_TRIANGLE_STRIP, 4);
    m_quad->setAttribute(0, 3, GL_FLOAT, GL_FALSE, 20, 0);
    m_quad->setAttribute(1, 2, GL_FLOAT, GL_FALSE, 20, 12);


    //SSAO create kernel
    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
    std::default_random_engine generator;

    for(int i=0; i <64; i++){
        glm::vec3 temp = glm::vec3(
                    randomFloats(generator) * 2.0 - 1.0,
                    randomFloats(generator) * 2.0 - 1.0,
                    randomFloats(generator) );
        temp = glm::normalize(temp);

        float scale = float(i) / float(64);
        scale = noiseLerp(0.1f, 1.0f, scale*scale);
        temp *= scale;
        m_kernel.push_back(temp);
    }

    //SSAO create noise
    genNoiseTexture();

}

void cauldron::update(int width, int height, glm::mat4 view, glm::mat4 projection, glm::mat4 model){
    m_width = width;
    m_height = height;
    m_view = view;
    m_projection = projection;
    m_model = model;

    m_depthFBO.reset(new FramebufferObject(m_width,m_height));
    m_positionFBO.reset(new FramebufferObject(m_width,m_height));
    m_normalFBO.reset(new FramebufferObject(m_width,m_height));
    m_ambientFBO.reset(new FramebufferObject(m_width,m_height));
    m_stoneUVFBO.reset(new FramebufferObject(m_width,m_height));
}

void cauldron::render(){
    //SSAO

//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_stoneUVFBO->bind();

//    glUseProgram(m_stoneUVProgram);
//    glUniformMatrix4fv(glGetUniformLocation(m_stoneUVProgram, "view"), 1, GL_FALSE, glm::value_ptr(m_view));
//    glUniformMatrix4fv(glGetUniformLocation(m_stoneUVProgram, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
//    glm::mat4 model = glm::mat4(0.25f, 0.0, 0.0, 0.0,
//                                0.0, 0.25f, 0.0, 0.0,
//                                0.0, 0.0, 0.25f, 0.0,
//                                0.0, 0.0, 0.0, 1.f);
//    glUniformMatrix4fv(glGetUniformLocation(m_stoneUVProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

//    m_sphere->draw();


    //store normal values (in screen space)
    m_normalFBO->bind();                                //bind the position FBO, we will now write to this
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_normalProgram);
    glUniformMatrix4fv(glGetUniformLocation(m_normalProgram, "view"), 1, GL_FALSE, glm::value_ptr(m_view));
    glUniformMatrix4fv(glGetUniformLocation(m_normalProgram, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
    glm::mat4 model = glm::mat4(0.25f, 0.0, 0.0, 0.0,
                                0.0, 0.25f, 0.0, 0.0,
                                0.0, 0.0, 0.25f, 0.0,
                                0.0, 0.0, 0.0, 1.f);
    glUniformMatrix4fv(glGetUniformLocation(m_normalProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glUniform1i(glGetUniformLocation(m_normalProgram, "texture1"), 0);

    m_cauldron->draw();

    //store depth values
    m_depthFBO->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_depthProgram);
    glUniformMatrix4fv(glGetUniformLocation(m_depthProgram, "view"), 1, GL_FALSE, glm::value_ptr(m_view));
    glUniformMatrix4fv(glGetUniformLocation(m_depthProgram, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
    glUniformMatrix4fv(glGetUniformLocation(m_depthProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    m_cauldron->draw();

//    // store position values
    m_positionFBO->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_positionProgram);
    glUniformMatrix4fv(glGetUniformLocation(m_positionProgram, "view"), 1, GL_FALSE, glm::value_ptr(m_view));
    glUniformMatrix4fv(glGetUniformLocation(m_positionProgram, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
    glUniformMatrix4fv(glGetUniformLocation(m_positionProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    m_cauldron->draw();


    GLfloat kernel[3*64];
    int index = 0;
    for(int i= 0; i<m_kernel.size(); i++){
        glm::vec3 temp = m_kernel[i];
        kernel[index] = temp.x;
        index++;
        kernel[index] = temp.y;
        index++;
        kernel[index] = temp.z;
        index++;
    }

    //store the occlusion values
    m_ambientFBO->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_ambientProgram);
    glUniformMatrix4fv(glGetUniformLocation(m_ambientProgram, "view"), 1, GL_FALSE, glm::value_ptr(m_view));
    glUniformMatrix4fv(glGetUniformLocation(m_ambientProgram, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
    glUniformMatrix4fv(glGetUniformLocation(m_ambientProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    //set the width and the height
    glUniform1f(glGetUniformLocation(m_ambientProgram, "width"), m_width);
    glUniform1f(glGetUniformLocation(m_ambientProgram, "height"), m_height);
    glUniform3fv(glGetUniformLocation(m_ambientProgram, "kernel"), 64, &kernel[0]);

    //set the texture
    glUniform1i(glGetUniformLocation(m_ambientProgram, "normal"), 0);
    glUniform1i(glGetUniformLocation(m_ambientProgram, "depth"), 1);
    glUniform1i(glGetUniformLocation(m_ambientProgram, "noise"), 2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_normalFBO->getColorTextureID());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_depthFBO->getColorTextureID());

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_noiseTexture);

    m_quad->draw();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    //final lighting pass
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);  //return to the default frame buffer (the screen)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_myPhongProgram);
    glUniformMatrix4fv(glGetUniformLocation(m_myPhongProgram, "view"), 1, GL_FALSE, glm::value_ptr(m_view));
    glUniformMatrix4fv(glGetUniformLocation(m_myPhongProgram, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
    glUniformMatrix4fv(glGetUniformLocation(m_myPhongProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glUniform1i(glGetUniformLocation(m_myPhongProgram, "position"), 0);
    glUniform1i(glGetUniformLocation(m_myPhongProgram, "normal"), 1);
    glUniform1i(glGetUniformLocation(m_myPhongProgram, "occlusion"), 2);
    glUniform1i(glGetUniformLocation(m_myPhongProgram, "stone"), 3);
    glUniform1i(glGetUniformLocation(m_myPhongProgram, "stoneUV"), 4);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_positionFBO->getColorTextureID());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_normalFBO->getColorTextureID());

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_ambientFBO->getColorTextureID());

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_textureID_stone);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, m_stoneUVFBO->getColorTextureID());


    glViewport(0, 0, m_width, m_height);
    m_quad->draw();

    glUseProgram(0);
}

GLfloat cauldron::noiseLerp(GLfloat a, GLfloat b, GLfloat f){
    return a + f * (b - a);
}

void cauldron::genNoiseTexture(){

    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
    std::default_random_engine generator;
    std::vector<glm::vec3> noiseVector;

    for(int i=0; i<16; i++){
        glm::vec3 temp = glm::vec3(
                    randomFloats(generator) * 2.0 - 1.0,
                    randomFloats(generator) * 2.0 - 1.0,
                    randomFloats(generator) );
        temp = glm::normalize(temp);
        noiseVector.push_back(temp);
    }

    glGenTextures(1, &m_noiseTexture);
    glBindTexture(GL_TEXTURE_2D, m_noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &noiseVector[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, m_noiseTexture);

}
