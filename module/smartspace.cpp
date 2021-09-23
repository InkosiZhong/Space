#include "smartspace.h"

SmartSpace::SmartSpace(SmartSpaceMode mode, ModuleDock* dock){
    setMode(mode);
    setModuleDock(dock);
}

void SmartSpace::setMode(SmartSpaceMode mode){
    m_mode = mode;
}

void SmartSpace::setModuleDock(ModuleDock* dock){
    if (!dock)return;
    m_module_dock = dock;
    qRegisterMetaType<Operations>("Operations");
    connect(this, SIGNAL(signalOperation(Operations)), m_module_dock, SLOT(onOperation(Operations)));
}

void SmartSpace::onOperation(Operations op){
    if (!m_history_op.empty() && m_history_op.back() == Clean){
        if (op == Clean)return;
        emit signalState(false);
        m_history_op.clear();
    }
    m_history_op.emplace_back(op);
}

void SmartSpace::AIInference(QPixmap* pixmap){
    // no AI yet
    if (m_mode != SPACE_AI || !pixmap)return;
    emit signalState(false);
    float conf[4] = {0};
    int cnt = 0;
    std::list<Operations>::iterator iter = m_history_op.begin();
    while(iter != m_history_op.end()){
        if (*iter == GetSrc || *iter == GetOperated ||
            *iter == GetFormula || *iter == GetOCR){
            conf[*iter] = 1;
            cnt++;
        }
        iter++;
    }
    for (int i = 0; i < 4; i++){
        if (conf[i])conf[i] = MAIN_CONF / cnt;
        else conf[i] = (1 - MAIN_CONF) / (4 - cnt);
    }
    conf[GetFormula] *= pixmap->width() / pixmap->height() / LATEX_SIZE_PROP_THREAD;
    conf[GetOCR] *= 0.5 / OCR_DENSITY_THREAD;
    m_infer_op.clear();
    int op = Clean;
    float max_conf = 0;
    for (int i = 0; i < 4 ; i++){
        if (conf[i] > max_conf){
            max_conf = conf[i];
            op = i;
        }
    }
    m_infer_op.emplace_back(Operations(op));
    m_infer_op.emplace_back(Clean);
    emit signalState(true);
}

const std::list<Operations>& SmartSpace::inference(){
    if (!m_history_op.empty() && m_history_op.back() == Clean)
    {
        switch (m_mode){
        case SPACE_DISBALE: break;
        case SPACE_AI: break;
        case SPACE_IMITATE: {
            m_infer_op.clear();
            m_infer_op = m_history_op;
            break;
        }
        }
    }
    return m_infer_op;
}

void SmartSpace::accept(){
    inference(); // avoid directly accept
    std::list<Operations>::iterator iter = m_infer_op.begin();
    while (iter != m_infer_op.end()){
        if (*iter == Clean)break;
        emit signalOperation(*iter);
        iter++;
    }
}
