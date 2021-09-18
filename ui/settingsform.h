#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QWidget>
#include <QCloseEvent>
#include <QDomDocument>
#include <QTextStream>
#include <QDebug>
#include <qvalidator.h>
#include "utils/xmlutil.h"
#include "constants.h"
#if __APPLE__
#include <QCoreApplication>
#endif

extern enum PromptLevel promptLevel;

namespace Ui {
class SettingsForm;
}

class SettingsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsForm(QWidget *parent = nullptr);
    ~SettingsForm();

Q_SIGNALS:
    void signalConfigSaved();

private slots:
    void on_hk_commit_button_clicked();
    void on_hk_init_button_clicked();
    void on_hk_cancel_button_clicked();
    void on_adv_commit_button_clicked();
    void on_adv_init_button_clicked();
    void on_adv_cancel_button_clicked();
    void on_gen_commit_button_clicked();
    void on_gen_init_button_clicked();
    void on_gen_cancel_button_clicked();
    void on_lu_threshold_slider_valueChanged(int value);
    void on_lu_threshold_edit_textChanged(const QString &val);
    void on_promptlevel_slider_valueChanged(int value);

private:
    void closeEvent(QCloseEvent *event);
    void initConfig();
    void loadConfig();
    void saveGeneralConfig();
    void saveAdvanceConfig();
    void saveHotKeyConfig();
    void save2File();

private:
    Ui::SettingsForm *m_ui;
#if _WIN32
    Util::XmlUtil m_xml_controller = Util::XmlUtil("config.xml", ".");
#elif __APPLE__
    Util::XmlUtil m_xml_controller = Util::XmlUtil(qApp->applicationDirPath() + "/config.xml", ".");
#endif
};

#endif // SETTINGSFORM_H
