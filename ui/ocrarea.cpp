#include "ocrarea.h"

OCRArea::OCRArea(const OCRWords* words, QWidget *parent)
{
    setup(words, parent);
}

OCRArea::OCRArea(const OCRArea& area): m_words(area.m_words){}

void OCRArea::onSetStyleSheet(bool checked){
    setStyleSheet(ocr_area_stylesheet[checked]);
}

void OCRArea::setup(const OCRWords* words, QWidget *parent){
    if (parent)setParent(parent);
    if (words)m_words = *words;
    float scale = Util::Util::windowsDpiScale();
    setGeometry(QRect(m_words.left / scale + 9, m_words.top / scale + 9, m_words.width / scale, m_words.height / scale));
    setStyleSheet(ocr_area_stylesheet[isChecked()]);
    connect(this, SIGNAL(toggled(bool)), this, SLOT(onSetStyleSheet(bool)));
    setFocusPolicy(Qt::NoFocus);
    show();
}

void OCRArea::mousePressEvent(QMouseEvent*){
    switch(checkState()){
    case Qt::Unchecked:
        setChecked(Qt::Checked);
        break;
    case Qt::Checked:
        setCheckState(Qt::Unchecked);
        break;
    default:
        break;
    }
}

void OCRArea::enterEvent(QEvent*){
    if (m_selecting){
        setChecked(!isChecked());
    }
}

void OCRArea::onSelecting(bool selecting){
    m_selecting = selecting;
}

void OCRArea::leaveEvent(QEvent*){

}

OCRWords OCRArea::getWordsWithLocation(){
    return m_words;
}

QString OCRArea::getWords(){
    return m_words.words;
}
