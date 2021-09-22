#include "moduledock.h"

ModuleDock::ModuleDock(QObject* parent) {
    ConfigPack cfg;
    setup(cfg);
}

void ModuleDock::setup(ConfigPack& cfg){
    m_bg_remover.setThreshold(cfg.lu_threshold);
    m_formula_extractor.setAppIdAndKey(cfg.mathpix_app_id, cfg.mathpix_app_key);
    m_ocr_handler.setAppTokenbyKey(cfg.baidu_ocr_api_key, cfg.baidu_ocr_secret_key);
}

void ModuleDock::rollBackOperation(){
    m_history_stack.pop();
    if (!m_history_stack.empty())emit signalSendMap(&m_history_stack.top());
}

void ModuleDock::clearAllOperation(){
    while (m_history_stack.size() > 2){
        m_history_stack.pop();
    }
    if (!m_history_stack.empty())emit signalSendMap(&m_history_stack.top());
}

void ModuleDock::onSetSrc(QPixmap* src){
    if (src->isNull())return;
    m_history_stack.clear();
    m_history_stack.push(src->copy());
    // NO more access src
    cv::Mat wo_bg = Utils::CvtFormatUtil::QPixmap2Mat(m_history_stack.top());
    wo_bg = m_bg_remover.handle(wo_bg);
    m_history_stack.push(Utils::CvtFormatUtil::Mat2QPixmap(wo_bg));
}

void ModuleDock::onOperation(Operations op){
    switch(op){
    case GetSrc: getSrc(); break;
    case GetOperated: getOperated(); break;
    case RollBack: rollBackOperation(); break;
    case ClearAll: clearAllOperation(); break;
    case Clean: clean(); break;
    case GetFormula: getFormula(); break;
    case GetOCR: getOCR(); break;
    }
}

bool ModuleDock::operationAvailable(int op){
    switch(op){
    case GetFormula:
        return m_formula_extractor.available();
    case GetOCR:
        return m_ocr_handler.available();
    default:
        return true;
    }
}

QPixmap* ModuleDock::getSrc(){
    if (m_history_stack.empty())return NULL;
    while (m_history_stack.size() > 1){
        m_history_stack.pop();
    }
    emit signalSendMap(&m_history_stack.top());
    return &m_history_stack.top();
}

QPixmap* ModuleDock::getOperated(){
    if (!m_history_stack.empty()){
        emit signalSendMap(&m_history_stack.top());
        return &m_history_stack.top();
    } else {
        return NULL;
    }
}

void ModuleDock::getFormula(){
    if (m_history_stack.empty())return;
    while (m_history_stack.size() > 1){
        m_history_stack.pop();
    }
    QString dir = "latex.jpg";
#if __APPLE__
    dir = qApp->applicationDirPath() + "/" + dir;
#endif
    if (!m_history_stack.empty())m_history_stack.top().save(dir);
    m_latex = m_formula_extractor.handle(dir);
    emit signalSendFormula(&m_latex);
}

void ModuleDock::getOCR(){
    if (m_history_stack.empty())return;
    while (m_history_stack.size() > 1){
        m_history_stack.pop();
    }
    if (!m_history_stack.empty()){
        m_ocr = m_ocr_handler.handle(m_history_stack.top());
    }
    emit signalSendOCR(&m_ocr);
}

void ModuleDock::clean(){
    m_history_stack.clear();
}
