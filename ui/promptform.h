#ifndef PROMPTFORM_H
#define PROMPTFORM_H

#include <QWidget>
#include <QtWidgets>

#include "constants.h"

extern enum PromptLevel promptLevel;

namespace Ui {
class PromptForm;
}

class PromptForm : public QWidget
{
    Q_OBJECT

public:
    explicit PromptForm(QWidget *parent = nullptr);
    ~PromptForm();

public:
    void setMinLevel(const int level = Lv_HINT);
    void setPrompt(QString prompt, const int level = Lv_HINT, QWidget* father = NULL);
    void hidePrompt();

private:
    Ui::PromptForm *m_ui;
    int m_min_level = Lv_HINT;
};

#endif // PROMPTFORM_H
