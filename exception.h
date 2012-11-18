/*!
 * @author Denis Vashchuk
*/

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QtCore/QString>

class Exception
{
public:
    Exception();
    Exception(QString message);

    QString getMessage();
protected:
    QString message;
};

class NotFoundException: public Exception
{
public:
    NotFoundException(): Exception()
    {}
    NotFoundException(QString message): Exception(message)
    {}
};

class OutOfIndexException: public Exception
{
public:
    OutOfIndexException(): Exception()
    {}
    OutOfIndexException(QString message): Exception(message)
    {}
};

#endif // EXCEPTION_H
