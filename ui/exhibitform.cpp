#include "exhibitform.h"
#include "ui_exhibitform.h"

bool section(OCRWords* last, OCRWords* now, int width){
    int last_btm = last->top + last->height;
    return now->left > now->height * 1.5 ||                         // 本行的左侧到左边框大于行高的1.5倍（1.5字）
           last->left + last->width  < width - now->height * 1.5 || // 上一行右侧到右边框大于行高的1.5倍（1.5字）
           now->height < last->height * 0.8 ||                      // 本行行高小于上一行的80%
           last->height < now->height * 0.8 ||                      // 上一行的行高小于当前行的80%
           last_btm - now->top > last->height * 0.1 ||              // 本行顶部到上一行底部距离大于上一行行高的10%
           now->top - last_btm > last->height * 0.8;                // 本行顶部到上一行底部距离大于上一行行高的80%
}

ExhibitForm::ExhibitForm(OCRInfoPack* ocr_pack):
    m_ui(new Ui::ExhibitForm)
{
    m_ui->setupUi(this);
    float scale = Util::Util::windowsDpiScale();
    if (ocr_pack->src){
        m_width = ocr_pack->src->width();
        setFixedSize(ocr_pack->src->width() / scale + 18, ocr_pack->src->height() / scale + 48);
        m_ui->label->setPixmap(*ocr_pack->src);
    }
    if (ocr_pack){
        std::list<OCRWords>::iterator iter = ocr_pack->ocr_result.begin();
        OCRWords* last = NULL;
        std::list<OCRWords> para_list;
        while(iter != ocr_pack->ocr_result.end()){
            OCRArea ocr_area(NULL, CG_LINE, this);
            m_ocr_areas[1].emplace_back(ocr_area);
            m_ocr_areas[1].back().setup(&*iter, CG_LINE, this);
            connect(this, SIGNAL(signalSelecting(bool)), &m_ocr_areas[1].back(), SLOT(onSelecting(bool)));
            m_whole_ocr_result += iter->words;
            para_list.emplace_back(*iter);
            last = &*iter;
            iter++;
            if (iter == ocr_pack->ocr_result.end() ||
                section(last, &*iter, m_width)){ // new para
                m_whole_ocr_result += "\n";
                OCRArea para_area(NULL, CG_PARA, this);
                m_ocr_areas[2].emplace_back(para_area);
                m_ocr_areas[2].back().setup(para_list, CG_PARA, this);
                connect(this, SIGNAL(signalSelecting(bool)), &m_ocr_areas[2].back(), SLOT(onSelecting(bool)));
                m_ocr_areas[2].back().hide();
                para_list.clear();
            }
        }
    }
}

ExhibitForm::~ExhibitForm()
{
    delete m_ui;
}

void ExhibitForm::closeEvent(QCloseEvent *event)
{
    emit signalFinishExhibit();
    this->deleteLater();
    event->accept();
}

void ExhibitForm::on_copy_button_clicked()
{
    std::list<OCRArea>::iterator iter = m_ocr_areas[m_coarse].begin();
    OCRArea* last_area = NULL;
    OCRWords words, last;
    QString ocr_result = "";
    while(iter != m_ocr_areas[m_coarse].end()){
        if (iter->isChecked()){
            if (last_area){
                last = last_area->getWordsWithLocation();
                words = iter->getWordsWithLocation();
                if (section(&last, &words, m_width))ocr_result += "\n";
            }
            ocr_result += iter->getWords();
            last_area = &*iter;
        }
        iter++;
    }
    if (ocr_result.isEmpty())ocr_result = m_whole_ocr_result;
    QClipboard *clip = QApplication::clipboard();
    clip->setText(ocr_result);
}

void ExhibitForm::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Control){
        emit signalSelecting(true);
    }
}

void ExhibitForm::keyReleaseEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Control){
        emit signalSelecting(false);
    }
}

void ExhibitForm::showCurrentCoarse(){
    for (CoarseGrained i = CG_WORD; i <= CG_PARA; i = CoarseGrained(i + 1)){
        std::list<OCRArea>::iterator iter = m_ocr_areas[i].begin();
        while (iter != m_ocr_areas[i].end()){
            iter->setVisible(i == m_coarse);
            iter++;
        }
    }
}


void ExhibitForm::on_next_pushButton_clicked()
{
    m_coarse = CoarseGrained((m_coarse + 1) % 3);
    showCurrentCoarse();
}


void ExhibitForm::on_prev_pushButton_clicked()
{
    m_coarse = CoarseGrained((m_coarse + 2) % 3);
    showCurrentCoarse();
}

