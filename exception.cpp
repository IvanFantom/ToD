#include "exception.h"

Exception::Exception()
{
}

Exception::Exception(QString text): message(text)
{
}

QString Exception::getMessage()
{
    return message;
}

