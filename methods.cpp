#include "methods.h"

Methods::Methods()
{

}

QVector<Point> Methods::GetStandarts()
{
    return standart;
}

int Methods::DirectionCosines(int x1, int y1, int x2, int y2)
{
    return 100*((float)x1*(float)x2+(float)y1*(float)y2)/(sqrt(((float)x1*(float)x1+(float)y1*(float)y1)*((float)x2*(float)x2+(float)y2*(float)y2)));
}

int Methods::EuclideanDistance(int x1, int y1, int x2, int y2)
{
    return sqrt(((float)x1-(float)x2)*((float)x1-(float)x2)+((float)y1-(float)y2)*((float)y1-(float)y2));
}

int Methods::TanimotoDistance(int x1, int y1, int x2, int y2)
{
    return 100*(float)(x1*x2+y1*y2)/(float)((x1*x1+y1*y1)+(x2*x2+y2*y2)-(x1*x2+y1*y2));
}

void Methods::StandartsCalculation(QVector<Point> sample)
{
    if(!standart.isEmpty())
        standart.clear();

    QVector<int> cl;                   //классы

    //выделение классов из выборки
    cl.append(sample.value(0).clas);
    for(int i=1;i<sample.size();i++)
    {
        if(!cl.contains(sample.value(i).clas))
            cl.append(sample.value(i).clas);
    }

    //рассчет эталона для каждого класса
    Point p;
    for(int i=0;i<cl.size();i++)
    {
        int sx=0,sy=0;
        int k=0;
        for(int j=0;j<sample.size();j++)
        {
            if(sample.value(j).clas==cl.value(i))
            {
                sx+=sample.value(j).x;
                sy+=sample.value(j).y;
                k++;
            }
        }
        p.x=sx/k;
        p.y=sy/k;
        p.clas=cl.value(i);
        standart.append(p);
    }
}

int Methods::Standarts(Point X,direction dir, int (*metric)(int, int, int, int))
{
    if(standart.isEmpty())
        return -1;

    QVector<int> dest;
    int value;
    int result;

    //рассчет расстояний между каждым эталоном и анализируемой точкой
    dest.append(metric(X.x,X.y,standart.value(0).x,standart.value(0).y));
    value=dest.value(0);result=standart.value(0).clas;
    for(int i=1;i<standart.size();i++)
    {
        dest.append(metric(X.x,X.y,standart.value(i).x,standart.value(i).y));
        if( (value<dest.value(i) && dir==MAX) || (value>dest.value(i) && dir==MIN) )
        {
            value = dest.value(i);
            result = standart.value(i).clas;
        }else if(value==dest.value(i))
            return -2;
    }

    return standart.value(result).clas;
}

int Methods::K_Neighbors(Point X,int K,QVector<Point> sample,direction dir, int (*metric)(int, int, int, int))
{
    if(K>sample.size() || !sample.size() || !K)
        return -1;

    QVector<int> dest,analiz;
    QVector<int> cl;                   //классы
    int result;

    //выделение классов из выборки
    cl.append(sample.value(0).clas);
    for(int i=0;i<sample.size();i++)
    {
        if(!cl.contains(sample.value(i).clas))
            cl.append(sample.value(i).clas);
    }

    //счетчики членов классов
    QVector<int> ccount;
    for(int i=0;i<cl.size();i++)
        ccount.append(0);

    //рассчет расстояний между анализируемой точкой и точками выборки
    for(int i=0;i<sample.size();i++)
    {
        dest.append(metric(X.x,X.y,sample.value(i).x,sample.value(i).y));
        analiz.append(dest.value(i));
    }
    //упорядочивание расстояний для простоты поиска K мин./макс. расстояний
    qSort(analiz);

    int j;
    if(dir==MAX)j=sample.size()-1;
    else j=0;

    //подсчет количества членов классов
    while( (j-K>=0 && dir==MAX) || (j<=K-1 && dir==MIN) )
    {
        ccount[cl.indexOf(sample[dest.indexOf(analiz.value(j))].clas)]+=1;

        if(dir==MAX)j--;
        else j++;
    }

    int value=ccount[0],valuei=0;

    //у какого класса больше членов?
    for(int i=1;i<ccount.size();i++)
    {
        if( value<ccount[i] )
        {
            value=ccount[i];
            valuei=i;
        }
        //else if(value==ccount[i])
        //    return -2;
    }

    return result=cl.value(valuei);
}

QVector<Point> Methods::findSplitLine(QVector<Point> srcpoints,int offsetY, int clas0, int clas1, int xMAX, int yMAX)
{
    QVector<Point> line,smartline,sample;
    int (*_metrix_)(int x1,int y1,int x2,int y2);
    Point P,P1,P2;
    float k;
    int minX,minY,maxX,maxY;
    int sumUpX,sumDownX,sumUpY,sumDownY;
    sumUpX=sumDownX=sumUpY=sumDownY=0;

    _metrix_ = Methods::EuclideanDistance;


    //выделение из множества точек обучающей выборки
    for(int i=0;i<srcpoints.size();i++)
    {
        if(srcpoints.value(i).classType==DEFINED && (srcpoints.value(i).clas==clas0 || srcpoints.value(i).clas==clas1))
            sample.append(srcpoints.value(i));
    }

    minX = maxX = sample.value(0).x;
    minY = maxY = sample.value(0).y;
    for(int i=1;i<sample.size();i++)
    {
        if(minX>sample.value(i).x)
            minX=sample.value(i).x;
        else if(maxX<sample.value(i).x)
            maxX=sample.value(i).x;

        if(minY>sample.value(i).y)
            minY=sample.value(i).y;
        else if(maxY<sample.value(i).y)
            maxY=sample.value(i).y;
    }

    StandartsCalculation(sample); //вычисление эталонов

    //проход по всей области классов
    for(int y=minY-1;y<maxY+1;y+=offsetY)
    {
        P.y=y;
        P.clas = -1;
        P.classType = UNDEFINED;

        for(int x=minX-1;x<maxX+1;x+=1)
        {
            P.x=x;
            if(Standarts(P,MIN,_metrix_)==-2)
                line.append(P);
        }
    }

    for(int i=0;i<line.size()/2;i++)
    {
        sumDownX+=line.value(i).x;
        sumDownY+=line.value(i).y;
    }
    for(int i=line.size()/2;i<line.size();i++)
    {
        sumUpX+=line.value(i).x;
        sumUpY+=line.value(i).y;
    }

    P1.x=sumDownX/(line.size()/2);
    P1.y=sumDownY/(line.size()/2);

    P2.x=sumUpX/( (line.size()) - (line.size()/2) );
    P2.y=sumUpY/( (line.size()) - (line.size()/2) );

    if(P1.y == P2.y)
    {
        P1.x=xMAX;
        P2.x=0;
    }
    else if(P1.x == P2.x)
    {
        P1.y=0;
        P2.y=yMAX;
    }
    else if(P1.x > P2.x)
    {
        float Dy = P2.y-P1.y;
        float Dx = P1.x-P2.x;

        while((P1.y > 0) && (P1.x < xMAX))
        {
            P1.y-=Dy;
            P1.x+=Dx;
        }
        while((P2.y < yMAX) && (P2.x > 0))
        {
            P2.y+=Dy;
            P2.x-=Dx;
        }

        if(P1.y<0 && P1.x<xMAX)
        {
            k = (P1.y+Dy-0)/(Dy);
            P1.x = P1.x - Dx*k;
            P1.y=0;
        }
        if(P1.y>0 && P1.x>xMAX)
        {
            k = (xMAX-(P1.x-Dx))/(Dx);
            P1.y = P1.y + Dy*k;
            P1.x=xMAX;
        }
        if(P2.y>yMAX && P2.x>0)
        {
            k = (yMAX-(P1.y-Dy))/(Dy);
            P2.x = P2.x +Dx*k;
            P2.y=yMAX;
        }
        if(P2.y<yMAX && P2.x<0)
        {
            k = (P2.x+Dx-0)/(Dx);
            P2.y = P2.y - Dy*k;
            P2.x=0;
        }
    }
    else if(P1.x < P2.x)
    {
        int Dy = P2.y-P1.y;
        int Dx = P2.x-P1.x;

        while((P1.y>0) && (P1.x>0))
        {
            P1.y-=Dy;
            P1.x-=Dx;
        }
        while((P2.y<yMAX) && (P2.x<xMAX))
        {
            P2.y+=Dy;
            P2.x+=Dx;
        }

        if(P1.y<0 && P1.x>0)
        {
            k=(P1.y+Dy-0)/(Dy);
            P1.x=P1.x-k*Dx;
            P1.y=0;
        }
        if(P1.y>0 && P1.x<0)
        {
            k=(P1.x+Dx-0)/(Dx);
            P1.y=P1.y+k*Dy;
            P1.x=0;
        }
        if(P2.y>yMAX && P2.x<xMAX)
        {
            k = (yMAX-(P2.y-Dy))/(Dy);
            P2.x=P2.x-k*Dx;
            P2.y=yMAX;
        }
        if(P2.y<yMAX && P2.x>xMAX)
        {
            k = (xMAX - (P2.x-Dx))/(Dx);
            P2.y=P2.y-k*Dy;
            P2.x=xMAX;
        }
    }

    smartline.append(P1);
    smartline.append(P2);

    return smartline;
}

ChartLine Methods::findSplitLines(QVector<Point> srcpoints, int dy,int xMAX,int yMAX)
{
    ChartLine lines;
    QVector<Point> defined;
    QVector<int> cl;

    for(int i=0;i<srcpoints.size();i++)
    {
        if(srcpoints.value(i).classType==DEFINED)
            defined.append(srcpoints.value(i));
    }
    //выделение классов
    cl.append(defined.value(0).clas);
    for(int i=1;i<defined.size();i++)
    {
        if(!cl.contains(defined.value(i).clas))
            cl.append(defined.value(i).clas);
    }

    //нахождение векторов точек разделяющих линий между всеми классами
    for(int i=0;i<cl.size();i++)
    {
        for(int j=i+1;j<cl.size();j++)
            lines.addLine(findSplitLine(defined,dy,cl.value(i),cl.value(j),xMAX,yMAX),cl.value(i),cl.value(j));
    }

    return lines;
}

QVector<Point> Methods::calculateClass(QVector<Point> points, CalculateMethod method, CalculateMetrix metrix)
{
    QVector<Point> sample;
    QVector<Point> Xpoints;
    int (*_metrix_)(int x1,int y1,int x2,int y2);
    direction dir;

    //выделение из множества точек обучающей выборки и точек, чей класс неизвестен
    for(int i=0;i<points.size();i++)
    {
        if(points.value(i).classType==DEFINED)
            sample.append(points.value(i));
        else Xpoints.append(points.value(i));
    }

    //определение метрики рассчета расстояний
    switch(metrix)
    {
        case CALCULATE_METRIX_EUCLIDEAN:
        {
            _metrix_ = Methods::EuclideanDistance;
            dir = MIN;
            break;
        }
        case CALCULATE_METRIX_DIRCOS:
        {
            _metrix_ = Methods::DirectionCosines;
            dir = MAX;
            break;
        }
        case CALCULATE_METRIX_TINIMOTO:
        {
            _metrix_ = Methods::TanimotoDistance;
            dir = MAX;
            break;
        }
    }

    //выбор метода определения принадлежности неизвестных точек и само определение принадлежности
    switch(method)
    {
        case CALCULATE_METHOD_STANTARD:
        {
            StandartsCalculation(sample);
            for(int i=0;i<Xpoints.size();i++)
            {
                Xpoints[i].clas=Standarts(Xpoints.value(i),dir,_metrix_);
                //! don't do it Xpoints[i].classType=DEFINED;
            }
            break;
        }
        case CALCULATE_METHOD_LINES:
        {
            StandartsCalculation(sample);
            for(int i=0;i<Xpoints.size();i++)
            {
                Xpoints[i].clas=Standarts(Xpoints.value(i),dir,_metrix_);
                //! don't do it Xpoints[i].classType=DEFINED;
            }
            break;
        }
        case CALCULATE_METHOD_K_NEIGBORDS:
        {
            int K = sample.size()/2;
            for(int i=0;(sample.size()+i)%2==0;i++)
                K = (sample.size()+i)/2;

            for(int i=0;i<Xpoints.size();i++)
            {
                Xpoints[i].clas=K_Neighbors(Xpoints.value(i),K,sample,dir,_metrix_);/////////////////////////////////////////////какое же число соседей?
                //! don't do it Xpoints[i].classType=DEFINED;
            }
            break;
        }
    }

    foreach(Point p, sample)
        Xpoints.append(p);

    return Xpoints;
}
