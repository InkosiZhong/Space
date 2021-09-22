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
    if (!m_history_op.empty() && op == Clean)return;
    if (!m_history_op.empty() && m_history_op.back() == Clean)m_history_op.clear();
    m_history_op.emplace_back(op);
}

void SmartSpace::AIInference(){

}

const std::list<Operations>& SmartSpace::inference(){
    m_infer_op.clear();
    switch (m_mode){
    case SPACE_DISBALE: break;
    case SPACE_AI: AIInference(); break;
    case SPACE_IMITATE: m_infer_op = m_history_op; break;
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
