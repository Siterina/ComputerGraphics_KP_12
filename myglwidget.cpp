#include "myglwidget.h"

#include <QtWidgets>
#include <QtOpenGL>
#include <QDebug>

#define pi 3.1415

MyGLWidget::MyGLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    a = 40, b = 60, count = 40;
    xRot = 0, yRot = 0, zRot = 0;
    xLightPos = 0, yLightPos = 0, zLightPos = 400;
    xMaterialColor = 0, yMaterialColor = 0.25, zMaterialColor = 0.3;
    xLightColor = 0.5, yLightColor = 0.6, zLightColor = 0.7;
    materialShiness = 10;
    xAmbientColor = 0.45, yAmbientColor = 0.35, zAmbientColor = 0.4;
    lastX = 0, lastY = 0;
    curTab = 0;
    carcas = false;

}

QVector3D MyGLWidget::ellipsPoint(double phi, double t1, double t2) {
    QVector3D temp;
    temp.setX((double)(t1));
    temp.setY((double)(a * cos(phi) + t2));
    temp.setZ((double)(b * sin(phi)));
    return temp;
}


void MyGLWidget::addToMassive(QVector <QVector3D> tempPoint, QVector3D n){
    unsigned long long size = tempPoint.size();
    for(unsigned long long i = 0; i < size; i++) {
        figureVertex.push_back(tempPoint[i].x());
        figureVertex.push_back(tempPoint[i].y());
        figureVertex.push_back(tempPoint[i].z());

        normal.push_back(n.x());
        normal.push_back(n.y());
        normal.push_back(n.z());

        index.push_back(index.size());
    }
}

void MyGLWidget::countFigurePoints() {

    figureVertex.clear();
    normal.clear();
    index.clear();
    double step_phi = pi / count * 2;
    double phi = pi/360;

    QVector <QVector3D> tempPoint;
    QVector3D n;

    for(int i = 0; i < count * (curve.size() - 1); i++) {
        for(int j = 0; j < count; phi += step_phi, j++) {

             tempPoint.push_back(ellipsPoint(phi, curveVertex[3*i], curveVertex[3*i + 1]));
             tempPoint.push_back(ellipsPoint(phi + step_phi, curveVertex[3*i], curveVertex[3*i + 1]));
             tempPoint.push_back(ellipsPoint(phi + step_phi, curveVertex[3*i + 3], curveVertex[3*i + 1 + 3]));
             tempPoint.push_back(ellipsPoint(phi, curveVertex[3*i + 3], curveVertex[3*i + 1 + 3]));

             n = QVector3D::normal(tempPoint[1] - tempPoint[0], tempPoint[3] - tempPoint[0]);

             addToMassive(tempPoint, n);
             tempPoint.clear();
        }
    }

/*    if(!carcas) {
        for(int i = 0; i < count * (curve.size() - 1); i++) {
            for(int j = 0; j < count; phi += step_phi, j++) {

                 tempPoint.push_back(ellipsPoint(phi, curveVertex[3*i], curveVertex[3*i + 1]));
                 tempPoint.push_back(ellipsPoint(phi + step_phi, curveVertex[3*i], curveVertex[3*i + 1]));
                 tempPoint.push_back(ellipsPoint(phi + step_phi, curveVertex[3*i + 3], curveVertex[3*i + 1 + 3]));
                 tempPoint.push_back(ellipsPoint(phi, curveVertex[3*i + 3], curveVertex[3*i + 1 + 3]));

                 n = QVector3D::normal(tempPoint[3] - tempPoint[0], tempPoint[1] - tempPoint[0]);

                 addToMassive(tempPoint, n);
                 tempPoint.clear();
            }
        }
    }*/
}

QPointF splain(double x, QPointF Pk, QPointF Pk1, QPointF Mk, QPointF Mk1) {
    QPointF result;
    double t = (x - Pk.x()) / (Pk1.x() - Pk.x());

    result = (2*t*t*t - 3*t*t + 1) * Pk +
            (t*t*t - 2*t*t + t) * (Pk1.x() - Pk.x()) * Mk +
            (-2*t*t*t + 3*t*t) * Pk1 +
            (t*t*t - t*t) * (Pk1.x() - Pk.x()) * Mk1;

    return result;
}


void MyGLWidget::curvePoints() {

    m.clear();
    curveVertex.clear();
    index.clear();


    for(int j = 0; j <= curve.size() - 1; j++) { // count differential vector
        if(j == 0)
            m.push_back((curve[j+1] - curve[j]) / (curve[j+1].x() - curve[j].x()));
        else if(j == curve.size() - 1)
            m.push_back((curve[j] - curve[j-1]) / (curve[j].x() - curve[j-1].x()));
        else
            m.push_back((curve[j+1] - curve[j-1]) / (curve[j+1].x() - curve[j-1].x()));

    }

  /*  qDebug() << mChange << " " << m.size() << " " << tensionPoint << endl;
    if(mChange) {
        m[tensionPoint].setX(tensionValueX);
        m[tensionPoint].setY(tensionValueY);
    }
    */
    QPointF toDraw;
    for(int j = 0; j < curve.size() - 1; j++) { // form vector to draw curve

        double step = fabs(curve[j+1].x() - curve[j].x()) / count;

        if(curve[j].x() < curve[j+1].x()) {
            for(double x = curve[j].x(); x < curve[j+1].x() + step; x+=step) {
                toDraw = splain(x, curve[j], curve[j+1], m[j], m[j+1]);
                curveVertex.push_back(toDraw.x());
                curveVertex.push_back(toDraw.y());
                curveVertex.push_back(0);
                index.push_back(index.size());
                if(curveVertex.size() >= (j+1) * count * 3 + 3)
                    break;
            }
        }
       /* else
            for(double x = curve[j].x(); x > curve[j+1].x() - step; x-=step) {
                toDraw = splain(x, curve[j], curve[j+1], m[j], m[j+1]);
                curveVertex.push_back(toDraw.x());
                curveVertex.push_back(toDraw.y());
                curveVertex.push_back(0);
                index.push_back(index.size());
            }*/
    }

}

void MyGLWidget::initializeGL() {

    glClearColor(xAmbientColor, yAmbientColor, zAmbientColor, 1);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);

    qDebug() << "trueInitialize" << endl;
}


void MyGLWidget::resizeGL(int width, int height){
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
}


void MyGLWidget::paintGL() {


    glClearColor(xAmbientColor, yAmbientColor, zAmbientColor, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0, 0, 0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-width()/2, width()/2, -height()/2, height()/2, -height()/2, height()/2);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    if(curve.size() > 1) {

        if(curTab == 1) {
            curvePoints();
            countFigurePoints();

            if(carcas) {
                glRotatef(-xRot,1,0,0);
                glRotatef(yRot,0,1,0);
                glRotatef(-zRot,0,0,1);

                glEnableClientState(GL_VERTEX_ARRAY);


                glVertexPointer(3,GL_FLOAT,0,figureVertex.data());
                glDrawElements(GL_LINE_STRIP,index.size(),GL_UNSIGNED_INT,index.data());

                glDisableClientState(GL_VERTEX_ARRAY);
            }
            else {
                GLfloat lightPosition[4] = {xLightPos, yLightPos, zLightPos, 1.0};
                glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

                GLfloat lightColor[4] = {xLightColor,yLightColor,zLightColor,1};
                glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);

                GLfloat ambientColor[4] = {xAmbientColor, yAmbientColor, zAmbientColor, 1};
                glLightfv(GL_LIGHT0, GL_AMBIENT, ambientColor);

                GLfloat materialColor[4] = {xMaterialColor,yMaterialColor,zMaterialColor,1};
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialColor);


                glEnable(GL_LIGHTING);
                glEnable(GL_LIGHT0);

                glRotatef(-xRot,1,0,0);
                glRotatef(yRot,0,1,0);
                glRotatef(-zRot,0,0,1);

                glEnableClientState(GL_VERTEX_ARRAY);
                glEnableClientState(GL_NORMAL_ARRAY);


                glVertexPointer(3,GL_FLOAT,0,figureVertex.data());
                glNormalPointer(GL_FLOAT,0,normal.data());

                glDepthRange (0, 1);
                glDrawElements(GL_QUADS,index.size(),GL_UNSIGNED_INT,index.data());

                glDisable(GL_LIGHTING);
                glDisable(GL_LIGHT0);

                glDisableClientState(GL_VERTEX_ARRAY);
                glDisableClientState(GL_NORMAL_ARRAY);
            }
        }
        else {

            glRotatef(-xRot,1,0,0);
            glRotatef(yRot,0,1,0);
            glRotatef(-zRot,0,0,1);

            glBegin(GL_POINTS);
            for(int i = 0; i < curve.size(); i++) {
                glVertex2f(curve[i].x(), curve[i].y());
            }
            glEnd();

            curvePoints();

            glEnableClientState(GL_VERTEX_ARRAY);

            glVertexPointer(3,GL_FLOAT,0,curveVertex.data());
            glDrawElements(GL_LINE_STRIP,index.size(),GL_UNSIGNED_INT,index.data());
            glDisableClientState(GL_VERTEX_ARRAY);
        }




     }

}




/*******************************************/
/*                Slots                    */
/*******************************************/

void MyGLWidget::mousePressEvent(QMouseEvent *mEvent) {
    if(mEvent->button() == Qt::LeftButton) {
        QPoint click (mEvent->x() - width() / 2, height()/2 - mEvent->y());
        curve.push_back(click);
    }
    else {
        if(curve.size() != 0)
            curve.pop_back();
    }
    updateGL();
}


void MyGLWidget::setXRotation(int angle) {
    xRot = (double)angle;
    updateGL();
}

void MyGLWidget::setYRotation(int angle) {
    yRot = (double)angle;
    updateGL();
}

void MyGLWidget::setZRotation(int angle) {
    zRot = (double)angle;
    updateGL();
}

void MyGLWidget::setXLightPos(int position) {
    xLightPos = (float)position;
    updateGL();
}

void MyGLWidget::setYLightPos(int position) {
    yLightPos = (float)position;
    updateGL();
}

void MyGLWidget::setZLightPos(int position) {
    zLightPos = (float)position;
    updateGL();
}

void MyGLWidget::setXMaterialColor(int color) {
    xMaterialColor = (float)color/255;
    updateGL();
}

void MyGLWidget::setYMaterialColor(int color) {
    yMaterialColor = (float)color/255;
    updateGL();
}

void MyGLWidget::setZMaterialColor(int color) {
    zMaterialColor = (float)color/255;
    updateGL();
}

void MyGLWidget::setXLightColor(int color) {
    xLightColor = (float)color/255;
    updateGL();
}

void MyGLWidget::setYLightColor(int color) {
    yLightColor = (float)color/255;
    updateGL();
}

void MyGLWidget::setZLightColor(int color) {
    zLightColor = (float)color/255;
    updateGL();
}

void MyGLWidget::setApproximation(int aCount) {
    count = aCount;
    updateGL();
}

void MyGLWidget::setXAmbientColor(int color) {
    xAmbientColor = (float)color/255;
    updateGL();
}

void MyGLWidget::setYAmbientColor(int color) {
    yAmbientColor = (float)color/255;
    updateGL();
}

void MyGLWidget::setZAmbientColor(int color) {
    zAmbientColor = (float)color/255;
    updateGL();
}

void MyGLWidget::clearWidget() {
    curve.clear();
    updateGL();
}

void MyGLWidget::changeA(int num) {
    a = num * 10;
    updateGL();
}

void MyGLWidget::changeB(int num) {
    b = num * 10;
    updateGL();
}

void MyGLWidget::changeTab(int tab){
    curTab = tab;
    updateGL();
}

void MyGLWidget::drawCarcas(bool change) {
    carcas = change;
    updateGL();
}
