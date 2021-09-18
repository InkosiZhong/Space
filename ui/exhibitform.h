#ifndef EXHIBITFORM_H
#define EXHIBITFORM_H

#include <QMainWindow>
#include <QPixmap>
#include <QRect>
#include <list>
#include <QCloseEvent>
#include <QClipboard>
#include "module/ocrhandler.h"
#include "utils/util.h"
#include "ocrarea.h"

namespace Ui {
class ExhibitForm;
}

class ExhibitForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit ExhibitForm(QPixmap* pixmap = NULL, OCRInfoPack* ocr_pack = NULL);

    ~ExhibitForm();

signals:
    void signalFinishExhibit();
    void signalSelecting(bool selecting);

private slots:
    void on_copy_button_clicked();

private:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::ExhibitForm *m_ui;
    std::list<OCRArea> ocr_areas;
    QString m_whole_ocr_result = "";
    int m_width;
};

#endif // EXHIBITFORM_H
