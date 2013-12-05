#include "../include/glwidget.h"
#include "../include/voxelSolver.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <QFile>
#include <QDebug>
#include <QTimer>
extern int qInitResources_shaders();
GLWidget::
    GLWidget(QWidget * parent)
    : QGLWidget(parent)
      , m_vboState(false)
{
    init();
}

GLWidget::
    GLWidget(const QGLFormat & format, QWidget * parent)
    : QGLWidget(format,parent)
      , m_vboState(false)
{
    init();
}


void GLWidget::
init() {
m_timer = new QTimer(this);
connect(m_timer, SIGNAL(timeout() ),this, SLOT( update() ));
       m_timer->start(16);
}


void GLWidget::
initializeGL() {
    glewInit();
    glClearColor(0,0,0,0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glDepthFunc(GL_LEQUAL);
    initializeTextures();

    m_pointProgram.reset(new ShaderProgram());
    QString shaderPrefix(":/vertexRendering/vertex.130");
    QString vertShaderName(shaderPrefix+tr(".v.glsl"));
    QString fragShaderName(shaderPrefix+tr(".f.glsl"));

    GLuint vId = compileShader(GL_VERTEX_SHADER, vertShaderName);
    GLuint fId = compileShader(GL_FRAGMENT_SHADER, fragShaderName);
    GLuint pId = m_pointProgram->programId;
    glUseProgram(pId);

    glAttachShader(pId,vId);
    glAttachShader(pId,fId);


    glLinkProgram(pId);
    m_viewMatrix =  glm::lookAt(glm::vec3(0,0,1),glm::vec3(0,0,0),glm::vec3(0,1,0));



}
GLuint GLWidget::compileShader(GLenum shaderType, const QString & fileName)
{

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "File not found: " << fileName;
        return false;
    }
    QString contents = file.readAll();
    GLuint shaderId = glCreateShader(shaderType);
    char * buf = new char[contents.length()+1];
    memcpy(buf,contents.toStdString().c_str(),contents.length());
    buf[contents.length()] = '\0';
    glShaderSource(shaderId,1, const_cast<const char **>(&buf),NULL);
    delete[] buf;
    glCompileShader(shaderId);
    GLint compile_ok;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compile_ok);
    if(compile_ok == GL_FALSE)
    {
        GLint log_length = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &log_length);
        char * log = new char[log_length];
        glGetShaderInfoLog(shaderId, log_length, NULL, log);
        qWarning() << log;
        delete[] log;
    }

    return shaderId;
}



void GLWidget::
initializeTextures() {

    glGenTextures(1,&m_dataTex);
    glEnable(GL_TEXTURE_3D);
    //    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, m_dataTex);

    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}



void GLWidget::
resizeGL(int w, int h) {
    glViewport(0,0,w,h);
    float near = 0.00001;
    float far = 10.0;
    float angle = 40;
    h = std::max(h,1);
    float aspect = float(w)/float(h);
    m_projectionMatrix = glm::perspective(angle,aspect,near,far);
    
}

void GLWidget::
paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(!curPkg().activeData)return;
    //glUseProgram(0);


    const int delta = m_time.elapsed() - m_lastTime;
    m_rotation += m_angularMomentum * (delta / 1000.0);
    m_lastTime += delta;

    m_pointProgram->bind();

    m_modelMatrix = glm::scale(glm::mat4(1),glm::vec3(0.5));
    m_modelMatrix = glm::rotate(m_modelMatrix, float(m_rotation.x()), glm::vec3(1.f,0.f,0.f));
    m_modelMatrix = glm::rotate(m_modelMatrix, float(m_rotation.y()), glm::vec3(0.f,1.f,0.f));
    m_modelMatrix = glm::rotate(m_modelMatrix, float(m_rotation.z()), glm::vec3(0.f,0.f,1.f));

    GLint vecl_loc = m_pointProgram->getAttribLocation("vecl");
    glUniformMatrix4fv(glGetUniformLocation(m_pointProgram->programId, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(m_viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(m_pointProgram->programId, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(m_modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(m_pointProgram->programId, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));
    glColor4f(1,1,1,1);
    glBegin(GL_POINTS);
    glVertex4f(.5,.5,.5,1);
    glVertex4f(0,0,0,1);
    glEnd();

    curPkg().activeData->bind(vecl_loc);
    curPkg().indices->render();
        //glDrawArrays(GL_POINTS, 0, curPkg().activeData->size);
    m_pointProgram->release();

}


void GLWidget::
vboRequest() {
    emit modifiableVBO(otherPkg());
}

void GLWidget::
swapVBO() {
    m_vboState = !m_vboState;
    /*
    if(curPkg().activeData) {
        auto&& pkg = curPkg();
        int numVerts = pkg.activeData->size;
        int numIndices = pkg.indices->size;
        VoxelData * a = new VoxelData[numVerts];
        glBindBuffer(GL_ARRAY_BUFFER,pkg.activeData->vbo);
        glGetBufferSubData(GL_ARRAY_BUFFER,0,numVerts*4*sizeof(float),(void*)a);
        std::cout << "here's all of the data: " << numVerts << std::endl;
        for(int i=0; i <numVerts; ++i) {
            std::cout << a[i].vec.transpose() << ": " << a[i].val << std::endl;
        }

            std::cout << a[5].vec.transpose()   << " ";
            std::cout << a[4].vec.transpose() << " ";
            std::cout << a[6].vec.transpose() << std::endl;
        delete a;
        return;
        unsigned int * b = new unsigned int[numIndices];
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,pkg.indices->ibo);
        glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,numIndices*sizeof(unsigned int),(void*)b);
        std::cout << "here's all of the indices : " << numIndices << std::endl;
        for(int i=0; i < numIndices; ++i,++i,++i) {
            std::cout << a[b[i]].vec.transpose()   << " ";
            std::cout << a[b[i+1]].vec.transpose() << " ";
            std::cout << a[b[i+2]].vec.transpose() << std::endl;
        }
        delete b;
    }
    */
    update();
}


void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        const QPointF delta = event->posF() - lastPos;
        lastPos=event->posF();
        const QVector3D angularImpulse = QVector3D(delta.y(), delta.x(), 0) * 0.1;

        m_rotation += angularImpulse;
        m_accumulatedMomentum += angularImpulse;

        event->accept();
        update();
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{

    m_mouseEventTime = m_time.elapsed();
    m_angularMomentum = m_accumulatedMomentum = QVector3D();
    event->accept();
    lastPos=event->posF();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{

    const int delta = m_time.elapsed() - m_mouseEventTime;
    m_angularMomentum = m_accumulatedMomentum * (1000.0 / qMax(1, delta));
    event->accept();
    update();
}



