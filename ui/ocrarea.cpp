#include "ocrarea.h"

OCRArea::OCRArea(const OCRWords* words, CoarseGrained cg, QWidget *parent)
{
    setup(words, cg, parent);
}

OCRArea::OCRArea(const OCRArea& area): m_words(area.m_words){}

void OCRArea::onSetStyleSheet(bool checked){
    setStyleSheet(ocr_area_stylesheet[m_coarse][checked]);
}

void OCRArea::setup(const OCRWords* words, CoarseGrained cg, QWidget *parent){
    m_coarse = cg;
    if (parent)setParent(parent);
    if (words)m_words = *words;
    float scale = Util::Util::windowsDpiScale();
    setGeometry(QRect(m_words.left / scale + 9, m_words.top / scale + 9, m_words.width / scale, m_words.height / scale));
    setStyleSheet(ocr_area_stylesheet[m_coarse][isChecked()]);
    connect(this, SIGNAL(toggled(bool)), this, SLOT(onSetStyleSheet(bool)));
    setFocusPolicy(Qt::NoFocus);
    show();
}

void OCRArea::setup(const std::list<OCRWords>& para, CoarseGrained cg, QWidget *parent){
    m_coarse = cg;
    if (parent)setParent(parent);
    m_words.top = para.front().top;
    m_words.left = para.front().left;
    int right = para.front().left + para.front().width;
    std::list<OCRWords>::const_iterator iter = para.begin();
    while(iter != para.end()){
        m_words.words += iter->words;
        if (iter->left < m_words.left){
            m_words.left = iter->left;
        }
        if (iter->left + iter->width > right){
            right = iter->left + iter->width;
        }
        iter++;
    }
    m_words.words += "\n";
    m_words.width = right - m_words.left;
    m_words.height = para.back().top + para.back().height - m_words.top;
    float scale = Util::Util::windowsDpiScale();
    setGeometry(QRect(m_words.left / scale + 9, m_words.top / scale + 9, m_words.width / scale, m_words.height / scale));
    setStyleSheet(ocr_area_stylesheet[m_coarse][isChecked()]);
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
