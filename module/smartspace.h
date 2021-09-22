#ifndef SMARTSPACE_H
#define SMARTSPACE_H

#include "constants.h"
#include <list>
#include <QObject>
#include <QDebug>
#include "moduledock.h"

class SmartSpace: public QObject
{
    Q_OBJECT
public:
    SmartSpace(SmartSpaceMode mode = SPACE_AI, ModuleDock* dock = NULL);

    void setMode(SmartSpaceMode mode);
    void setModuleDock(ModuleDock* dock);

    const std::list<Operations>& inference();

    void accept();

signals:
    void signalOperation(Operations op);

public slots:
    void onOperation(Operations op);

private:
    void AIInference();

private:
    SmartSpaceMode m_mode;
    ModuleDock* m_module_dock;
    std::list<Operations> m_history_op;
    std::list<Operations> m_infer_op;
};


#endif // SMARTSPACE_H
