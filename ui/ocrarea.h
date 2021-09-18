#ifndef OCRAREA_H
#define OCRAREA_H

#include <QCheckBox>
#include <QEvent>
#include <QMouseEvent>
#include "module/ocrhandler.h"
#include "utils/util.h"
#include "constants.h"

class OCRArea : public QCheckBox
{
    Q_OBJECT
public:
    explicit OCRArea(const OCRWords* words = nullptr, QWidget *parent = nullptr);

    OCRArea(const OCRArea& area);

    void setup(const OCRWords* words = nullptr, QWidget *parent = nullptr);

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

};

#endif // OCRAREA_H
