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

ExhibitForm::ExhibitForm(QPixmap* pixmap, OCRInfoPack* ocr_pack):
    m_ui(new Ui::ExhibitForm)
{
    m_ui->setupUi(this);
    float scale = Util::Util::windowsDpiScale();
    if (pixmap){
        m_width = pixmap->width();
        setFixedSize(pixmap->width() / scale + 18, pixmap->height() / scale + 48);
        m_ui->label->setPixmap(*pixmap);
    }
    if (ocr_pack){
        std::list<OCRWords>::iterator iter = ocr_pack->ocr_result.begin();
        OCRWords* last = NULL;
        while(iter != ocr_pack->ocr_result.end()){
            OCRArea ocr_area(NULL, this);
            ocr_areas.emplace_back(ocr_area);
            ocr_areas.back().setup(&*iter, this);
            connect(this, SIGNAL(signalSelecting(bool)), &ocr_areas.back(), SLOT(onSelecting(bool)));
            m_whole_ocr_result += iter->words;
            last = &*iter;
            iter++;
            if (iter != ocr_pack->ocr_result.end() &&
                section(last, &*iter, m_width))m_whole_ocr_result += "\n";
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
    std::list<OCRArea>::iterator iter = ocr_areas.begin();
    OCRArea* last_area = NULL;
    OCRWords words, last;
    QString ocr_result = "";
    while(iter != ocr_areas.end()){
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

