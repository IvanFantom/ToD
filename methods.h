/*!
 * @author Ivan Zaharchuk
*/

#ifndef METHODS_H
#define METHODS_H

#include "point.h"
#include "chartline.h"
#include "enums.h"

#include <math.h>
#include <QVector>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <iostream>
using namespace std;

enum direction{MIN,MAX};

class Methods
{
private:
    QVector<Point> standart; //������� �������

    void StandartsCalculation(QVector<Point> sample);          //���������� ��������
public:
    Methods();

    QVector<Point> GetStandarts();                             //���������� ������� ��������

    static int DirectionCosines(int x1,int y1,int x2,int y2);  //������������ ��������
    static int EuclideanDistance(int x1,int y1,int x2,int y2); //��������� ����������
    static int TanimotoDistance(int x1,int y1,int x2,int y2);  //���������� ��������

    int Standarts(Point X,direction dir,int (*metric)(int x1,int y1,int x2,int y2));                               //����� ��������
    int K_Neighbors(Point X,int K,QVector<Point> sample,direction dir,int (*metric)(int x1,int y1,int x2,int y2)); //����� k-��������� �������

    QVector<Point> calculateClass(QVector<Point> points,CalculateMethod method,CalculateMetrix metrix);

    QVector<Point> findSplitLine(QVector<Point> srcpoints,int offsetY,int clas0, int clas1,int xMAX,int yMAX);
    ChartLine findSplitLines(QVector<Point> srcpoints,int dy, int xMAX, int yMAX);
};

#endif // METHODS_H
