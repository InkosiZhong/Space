#ifndef MODULEDOCK_H
#define MODULEDOCK_H

#include <QDebug>
#include <QPixmap>
#include <QStack>
#include "opencv2/opencv.hpp"
#include "utils/cvtformatutil.h"
#include "constants.h"
#if __APPLE__
#include <QCoreApplication>
#endif

// modules
#include "module/backgroundremover.h"
#include "module/latexformulaextractor.h"
#include "module/ocrhandler.h"

struct ConfigPack
{
    const int lu_threshold = 127;
    const QString mathpix_app_id = "", mathpix_app_key = "";
    const QString baidu_ocr_api_key = "", baidu_ocr_secret_key = "";
    ConfigPack() {};
    ConfigPack(const int lu, const QString math_id, const QString math_key,
               const QString ocr_api_key, const QString ocr_secret_key):
        lu_threshold(lu), mathpix_app_id(math_id), mathpix_app_key(math_key),
        baidu_ocr_api_key(ocr_api_key), baidu_ocr_secret_key(ocr_secret_key){}
};


class ModuleDock: public QObject
{
    Q_OBJECT
public:
    ModuleDock(QObject* parent = nullptr);
    void setup(ConfigPack& cfg);

Q_SIGNALS:
    void signalSendMap(QPixmap* map);
    void signalSendFormula(LatexInfoPack* latex);
    void signalSendOCR(OCRInfoPack* ocr);

public slots:
    void onSetSrc(QPixmap* src);
    void onOperation(Operations);

public:
    bool operationAvailable(int);
    void rollBackOperation();       // get the previous one
    void clearAllOperation();       // get the second last one
    void clean();                   // clean to original state
    QPixmap* getSrc();              // get the last one
    QPixmap* getOperated();         // get the top one
    void getFormula();
    void getOCR();

private:
    QStack<QPixmap> m_history_stack;
    BackgroundRemover m_bg_remover;
    LatexFormulaExtractor m_formula_extractor;
    LatexInfoPack m_latex;
    OCRHandler m_ocr_handler;
    OCRInfoPack m_ocr;
};

#endif // MODULEDOCK_H
