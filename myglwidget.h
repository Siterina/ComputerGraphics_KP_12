#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QWidget>
#include <QGLWidget>

class MyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = 0);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    QVector3D ellipsPoint(double phi, double t1, double t2);
    void addToMassive(QVector <QVector3D> tempPoint, QVector3D n);
    void countFigurePoints();
    void curvePoints();

    void mousePressEvent(QMouseEvent *mEvent);

private:

    QVector <GLfloat> figureVertex;
    QVector <GLfloat> curveVertex;
    QVector <GLfloat> normal;
    QVector <GLuint> index;

    float xRot;
    float yRot;
    float zRot;

    float xLightPos;
    float yLightPos;
    float zLightPos;

    float xMaterialColor;
    float yMaterialColor;
    float zMaterialColor;

    float xLightColor;
    float yLightColor;
    float zLightColor;

    float xAmbientColor;
    float yAmbientColor;
    float zAmbientColor;

    double materialShiness;
    double a, b, c;
    double count;

    double lastX, lastY;

    QVector <QPointF> curve;
    QVector <QPointF> m;

    int curTab;
    bool carcas;


signals:

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

    void setXLightPos(int position);
    void setYLightPos(int position);
    void setZLightPos(int position);

    void setXMaterialColor(int color);
    void setYMaterialColor(int color);
    void setZMaterialColor(int color);

    void setXLightColor(int color);
    void setYLightColor(int color);
    void setZLightColor(int color);

    void setXAmbientColor(int color);
    void setYAmbientColor(int color);
    void setZAmbientColor(int color);

    void setApproximation(int aCount);

    void clearWidget();

    void changeA(int num);
    void changeB(int num);
    void changeC(double num);

    void changeTab(int tab);
    void drawCarcas(bool change);


};

#endif // MYGLWIDGET_H
