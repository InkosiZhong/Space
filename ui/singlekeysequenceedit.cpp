#include "singlekeysequenceedit.h"
#include "singlekeysequenceedit.h"
#include <QKeyEvent>

SingleKeySequenceEdit::SingleKeySequenceEdit(QWidget *parent) : QKeySequenceEdit(parent) { }

SingleKeySequenceEdit::~SingleKeySequenceEdit() { }

void SingleKeySequenceEdit::keyPressEvent(QKeyEvent *pEvent)
{
    if (pEvent->key() == Qt::Key_Space){
        QMessageBox::warning(this,
                tr("Space"),
                tr("[Space]为智能键，无法设置为快捷键"),
                QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    QKeySequenceEdit::keyPressEvent(pEvent);
    QString val = keySequence().toString().split(", ").first();
    QKeySequence seq(QKeySequence::fromString((val == "Backspace") ? "" : val));
    setKeySequence(seq);
}
