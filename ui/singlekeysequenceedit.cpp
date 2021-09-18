#include "singlekeysequenceedit.h"
#include "singlekeysequenceedit.h"
#include <QKeyEvent>

SingleKeySequenceEdit::SingleKeySequenceEdit(QWidget *parent) : QKeySequenceEdit(parent) { }

SingleKeySequenceEdit::~SingleKeySequenceEdit() { }

void SingleKeySequenceEdit::keyPressEvent(QKeyEvent *pEvent)
{
    QKeySequenceEdit::keyPressEvent(pEvent);
    QKeySequence seq(QKeySequence::fromString(keySequence().toString().split(", ").first()));
    setKeySequence(seq);
}
