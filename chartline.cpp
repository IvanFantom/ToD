#include "chartline.h"

ChartLine::ChartLine()
{
}

void ChartLine::addLine(const QVector<Point> &line, int class0, int class1)
{
    LineInfo inf;
    inf.class0 = class0;
    inf.class1 = class1;

    lines.append(line);
    linesInfo.append(inf);
}


QVector<Point> &ChartLine::lineAt(int index)
{
    return lines[index];
}

ChartLine::LineInfo ChartLine::lineInfoAt(int index)
{
    return linesInfo[index];
}

int ChartLine::count()
{
    return lines.count();
}

void ChartLine::removeAll()
{
    lines.clear();
    linesInfo.clear();
}

void ChartLine::operator=(ChartLine &line)
{
    removeAll();
    for(int i=0; i<line.count(); i++) {
        addLine(line.lineAt(i), line.lineInfoAt(i).class0, line.lineInfoAt(i).class1);
    }

}


void ChartLine::removeLine(int index)
{
    lines.remove(index);
    linesInfo.remove(index);
}
