/*!
 * @author Denis Vashchuk
 *  GUI Dialog for new point properies define
 *
*/

#ifndef POINTADDDIALOG_H
#define POINTADDDIALOG_H

#include <QDialog>
#include "point.h"

namespace Ui {
class PointAddDialog;
}

class PointAddDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PointAddDialog(QWidget *parent = 0);
    ~PointAddDialog();
    
    int getClassNumber();
    ClassType getClassType();
private:
    Ui::PointAddDialog *ui;
};

#endif // POINTADDDIALOG_H
