#include "promptform.h"
#include "ui_promptform.h"

PromptForm::PromptForm(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::PromptForm)
{
    m_ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
}

PromptForm::~PromptForm()
{
    delete m_ui;
}

void PromptForm::setMinLevel(const int level){
    m_min_level = level;
}

void PromptForm::setPrompt(QString prompt, const int level, QWidget* father) {
    if (level < m_min_level || father == NULL || father->isHidden())return;
    this->setParent((QWidget*)(father->parent()));
    m_ui->prompt_label->setText(level_prefix[level] + prompt);
    this->setStyleSheet(level_stylesheet[level]);
    show();
}

void PromptForm::hidePrompt(){
    this->setParent(NULL);
    hide();
}
