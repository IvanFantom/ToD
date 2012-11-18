/*!
 * @author Denis Vashchuk
*/


#ifndef CLASSCOLOR_H
#define CLASSCOLOR_H

#include <QtGui/QColor>
#define MAX 50

class ClassColor {
public:

    static QColor colorTable[];
    static QColor borderColor;
    static QColor shadowColor;

    static QColor getShadowColor(QColor foregroundColor);


    static QColor classToColor(int classNumber)
    {
        return (classNumber < 14 && classNumber != -1 )?(colorTable[classNumber]):(Qt::gray);
    }
private:
     static int getInt(int k);
};

QColor ClassColor::shadowColor = QColor(150, 150, 150);

QColor ClassColor::borderColor = QColor(0xf2f2f2);

QColor ClassColor::colorTable[] = { QColor(0x4f, 0x81, 0xbd),
                                    QColor(0xc0,0x50, 0x4d),
                                    Qt::darkGreen,
                                    Qt::blue,
                                    Qt::darkBlue,
                                    Qt::cyan,
                                    Qt::red,
                                    Qt::darkCyan,
                                    Qt::magenta,
                                    Qt::darkMagenta,
                                    Qt::yellow,
                                    Qt::darkYellow,
                                    Qt::darkGray,
                                    QColor(255,200,200)
                                  };

QColor ClassColor::getShadowColor(QColor foregroundColor)
{
    int r = foregroundColor.red();
    int g = foregroundColor.green();
    int b = foregroundColor.blue();

    int base = qMin(r, g);
    base = qMin(b, base);

    r-=base;
    g-=base;
    b-=base;

    r = getInt(r);
    g = getInt(g);
    b = getInt(b);

    return QColor(r+shadowColor.red(), g+shadowColor.green(), b+shadowColor.blue());
}

int ClassColor::getInt(int k)
{
    return (k>MAX)?(MAX):(k);
}

#endif
