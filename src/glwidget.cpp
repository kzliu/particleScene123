#include "glwidget.h"
#include "settings.h"

#include "cs123_lib/resourceloader.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr

#include <iostream>
#include <QImage>
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>
#include "openglshape.h"
#include "ParticleSystem.h"
#include "cauldron.h"

GLWidget::GLWidget(QGLFormat format, QWidget *parent)
    : QGLWidget(format, parent),
      m_width(width()), m_height(height()),
      m_square(nullptr),
      m_solidProgramID(0), m_gradientProgramID(0), m_textureProgramID(0), m_updateProgramID(0),
      m_drawProgramID(0),
      m_FBO1(nullptr), m_FBO2(nullptr),
      m_textureID(0),
      m_textureID_stone(0), m_noiseTexture(0),
      m_angleX(-0.5f), m_angleY(0.5f), m_zoom(4.f),
      m_cauldron(nullptr),
      m_timer(this),
      m_fps(10.f),
      m_increment(0),
      m_particles(nullptr),
      m_vertex_vector{}
{
    // Set up 60 FPS draw loop.
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
    m_timer.start(1000.0f / m_fps);
}

GLWidget::~GLWidget()
{
    // TODO: Clean up GPU memory.
    glDeleteTextures(1,&m_textureID);

}


void GLWidget::initializeGL()
{
    ResourceLoader::initializeGlew();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Defines the color the screen will be cleared to.

    // Creates the three shader programs.
    m_solidProgramID = ResourceLoader::createShaderProgram(":/shaders/solid.vert", ":/shaders/solid.frag");
    m_gradientProgramID = ResourceLoader::createShaderProgram(":/shaders/gradient.vert", ":/shaders/gradient.frag");
    m_textureProgramID = ResourceLoader::createShaderProgram(":/shaders/texture.vert", ":/shaders/texture.frag");
    m_updateProgramID = ResourceLoader::createShaderProgram(":/shaders/texture.vert", ":/shaders/update.frag");
    m_drawProgramID = ResourceLoader::createShaderProgram(":/shaders/draw.vert", ":/shaders/draw.frag");

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

    qDebug() << loadOBJ("/gpfs/main/home/kzliu/course/cs123/hmm/cauldron_uv.obj", m_vertex_vector);

    QImage image;
    bool success = image.load(QString::fromStdString("/gpfs/main/home/kzliu/course/cs123/hmm/images/normal_stone.png"));
    qDebug() << success;

    glGenTextures(1,&m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    QImage image2;
    bool success2 = image2.load(QString::fromStdString("/gpfs/main/home/kzliu/course/cs123/hmm/images/stone.png"));
    qDebug() << success2;

    glGenTextures(1,&m_textureID_stone);
    glBindTexture(GL_TEXTURE_2D, m_textureID_stone);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image2.width(), image2.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image2.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glm::mat4 model = glm::mat4(0.05f, 0.0, 0.0, 0.0,
                                0.0, 0.05f, 0.0, 0.0,
                                0.0, 0.0, 0.05f, 0.0,
                                0.0, 0.0, 0.0, 1.f);
    m_cauldron.reset(new cauldron(this->width(), this->height(), m_view, m_projection, model, m_vertex_vector, m_textureID, m_textureID_stone));
}

void GLWidget::paintGL()
{
    glm::mat4 model = glm::mat4(0.05f, 0.0, 0.0, 0.0,
                                0.0, 0.05f, 0.0, 0.0,
                                0.0, 0.0, 0.05f, 0.0,
                                0.0, 0.0, 0.0, 1.f);
//    cauldron *caul = new cauldron(this->width(), this->height(), m_view, m_projection, model, m_vertex_vector);


//    float time = m_increment++ / (float) m_fps;      // Time in seconds
    float s = floor(pow(255, 2) / std::max(this->width(), this->height()) / 3);
    glm::vec2 scale = glm::vec2(s, s*100);
    m_particles.reset(new ParticleSystem(20, 10, this->width(), this->height(), scale[0], scale[1], 10.f, qRgba(100,100,100,150)));

    switch (settings.shaderProgram) {
    case SOLID_SHADER_PROGRAM:
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        m_cauldron->update(this->width(), this->height(), m_view, m_projection, model);
        m_cauldron->render();


//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        GLuint m_colorTextureID;
//        glGenTextures(1, &m_colorTextureID);
//        glBindTexture(GL_TEXTURE_2D, m_colorTextureID);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width(), this->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//        m_FBO1->attach(m_colorTextureID);


//        glUseProgram(m_gradientProgramID);

//        m_square->draw();

//        glBindTexture(GL_TEXTURE_2D, m_colorTextureID);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);



        break;

    case GRADIENT_SHADER_PROGRAM:
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_particles->update(m_updateProgramID);
        m_particles->draw(m_drawProgramID);
        // TODO (Task 5): Draw the square using m_gradientProgramID.
//        glUseProgram(m_gradientProgramID);
//        m_square->draw();
        break;
    case TEXTURE_SHADER_PROGRAM:
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        // TODO (Task 8): Draw the square using m_textureProgramID.

////        particles->update(m_FBO1, m_textureProgramID, m_square); <- trouble passing the fbo
//        particles->update(m_updateProgramID, m_square);
////        glUseProgram(m_textureProgramID);
//        glBindTexture(GL_TEXTURE_2D, particles->get_p0texture());
//        m_square->draw();
//        glBindTexture(GL_TEXTURE_2D,0);
        break;
    }
    glUseProgram(0);

}

void GLWidget::resizeGL(int w, int h)
{
    m_width = w;
    m_height = h;

    glViewport(0, 0, m_width, m_height);

    m_FBO1.reset(new FramebufferObject(w,h));
    m_FBO2.reset(new FramebufferObject(w,h));

    rebuildMatrices();
}

/** Repaints the canvas. Called 60 times per second. */
void GLWidget::tick()
{
    update();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_prevMousePos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_angleX += 100 * (event->x() - m_prevMousePos.x()) / (float) width();
    m_angleY += 100 * (event->y() - m_prevMousePos.y()) / (float) height();
    m_prevMousePos = event->pos();
    rebuildMatrices();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    m_zoom -= event->delta() / 20.f;
    rebuildMatrices();
}

void GLWidget::rebuildMatrices()
{
    m_view = glm::translate(glm::vec3(0, 0, -m_zoom)) *
             glm::rotate(m_angleY, glm::vec3(1,0,0)) *
             glm::rotate(m_angleX, glm::vec3(0,1,0));

    m_projection = glm::perspective(0.8f, (float)width()/height(), 0.1f, 1000.f);
    update();
}

//parser
bool GLWidget::loadOBJ(const char * path,  std::vector<GLfloat> &vertex_vector){

    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;


    FILE * file = fopen(path, "r");
    if(file == NULL ){
        std::cout << "Could not load file." << std::endl;
        return false;
    }

    while(1){

        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;

        if ( strcmp( lineHeader, "v" ) == 0 ){
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices.push_back(vertex);

        }else if ( strcmp( lineHeader, "vt" ) == 0 ){
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            temp_uvs.push_back(uv);

        }else if ( strcmp( lineHeader, "vn" ) == 0 ){
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);

        }else if ( strcmp( lineHeader, "f" ) == 0 ){
            //std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                    &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                    &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

            if (matches != 9){
                std::cout << "matches error" << std::endl;
                return false;
            }

            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);

            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);

            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }

    assert(vertexIndices.size() == normalIndices.size());

    //calculate tangents

    glm::vec3 tangents[vertexIndices.size()] = {glm::vec3(0)};
    for (unsigned int i = 0; i < vertexIndices.size(); i+=3){
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int vertexIndex1 = vertexIndices[i+1];
        unsigned int vertexIndex2 = vertexIndices[i+2];
        unsigned int uvIndex = uvIndices[i];
        unsigned int uvIndex1 = uvIndices[i+1];
        unsigned int uvIndex2 = uvIndices[i+2];


        glm::vec3 tangent = calculateTan(temp_vertices[vertexIndex-1], temp_vertices[vertexIndex1-1], temp_vertices[vertexIndex2-1], temp_uvs[uvIndex-1], temp_uvs[uvIndex1-1], temp_uvs[uvIndex2-1]);
        tangents[vertexIndex-1] += tangent;
        tangents[vertexIndex1-1] += tangent;
        tangents[vertexIndex2-1] += tangent;
    }

    for(unsigned int i=0; i<vertexIndices.size(); i++ ){
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int normalIndex = normalIndices[i];
        unsigned int uvIndex = uvIndices[i];

        glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
        glm::vec3 normal = temp_normals[ normalIndex-1 ];
        glm::vec2 uv = temp_uvs[ uvIndex-1 ];
        glm::vec3 tangent = glm::normalize(tangents[vertexIndex-1]);

        vertex_vector.push_back(vertex.x/2.f);
        vertex_vector.push_back(vertex.y/2.f);
        vertex_vector.push_back(vertex.z/2.f);

        vertex_vector.push_back(uv.x);
        vertex_vector.push_back(uv.y);

        vertex_vector.push_back(normal.x);
        vertex_vector.push_back(normal.y);
        vertex_vector.push_back(normal.z);

        vertex_vector.push_back(tangent.x);
        vertex_vector.push_back(tangent.y);
        vertex_vector.push_back(tangent.z);

    }

    return true;
}

glm::vec3 GLWidget::calculateTan(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec2 uv0, glm::vec2 uv1, glm::vec2 uv2){

     glm::vec3 tangent = {glm::vec3(0.f)};

    glm::vec3 Edge1 = v1 - v0;
    glm::vec3 Edge2 = v2 - v0;

    float DeltaU1 = uv1.x - uv0.x;
    float DeltaV1 = uv1.y - uv0.y;
    float DeltaU2 = uv2.x - uv0.x;
    float DeltaV2 = uv2.y - uv0.y;

    float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

    tangent.x = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
    tangent.y = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
    tangent.z = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);

    return tangent;


}
