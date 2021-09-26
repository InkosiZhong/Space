#ifndef OCRAREA_H
#define OCRAREA_H

#include <QCheckBox>
#include <QEvent>
#include <QMouseEvent>
#include "module/ocrhandler.h"
#include "utils/util.h"
#include "constants.h"
#include <list>

class OCRArea : public QCheckBox
{
    Q_OBJECT
public:
    explicit OCRArea(const OCRWords* words = nullptr, CoarseGrained cg = CG_LINE, QWidget *parent = nullptr);

    OCRArea(const OCRArea& area);

    void setup(const OCRWords* words = nullptr, CoarseGrained cg = CG_LINE, QWidget *parent = nullptr);

    void setup(const std::list<OCRWords>& para, CoarseGrained cg = CG_PARA, QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent* e);

    void enterEvent(QEvent* e);

    void leaveEvent(QEvent* e);

    OCRWords getWordsWithLocation();

    QString getWords();

public slots:
    void onSelecting(bool selecting);
    void onSetStyleSheet(bool checked);

private:
    OCRWords m_words;
    bool m_selecting = false;
    CoarseGrained m_coarse;

};

#endif // OCRAREA_H
