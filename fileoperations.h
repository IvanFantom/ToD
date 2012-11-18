/*!
 * @author Ivan Zaharchuk
 * @author Denis Vashchuk
 * Class for work with split lines in chart
 *
*/

#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <QtCore/QVector>
#include <QtCore/QString>
#include "point.h"

class FileOperations
{
public:
    FileOperations();
    static void WriteFile(QVector<Point> points,QString filename);
    static QVector<Point> ReadFile(QString filename);
};

#endif // FILEOPERATIONS_H
