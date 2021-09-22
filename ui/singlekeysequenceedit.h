#ifndef SINGLEKEYSEQUENCEEDIT_H
#define SINGLEKEYSEQUENCEEDIT_H

#include <QKeySequenceEdit>
#include <QMessageBox>

class QKeyEvent;
class SingleKeySequenceEdit : public QKeySequenceEdit
{
Q_OBJECT
public:
    explicit SingleKeySequenceEdit(QWidget *parent = 0);
    ~SingleKeySequenceEdit();
protected:
    void keyPressEvent(QKeyEvent *pEvent);
};

#endif // SINGLEKEYSEQUENCEEDIT_H
