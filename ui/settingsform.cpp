#include "settingsform.h"
#include "ui_settingsform.h"

SettingsForm::SettingsForm(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::SettingsForm)
{
    m_ui->setupUi(this);
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
    loadConfig();
    m_ui->lu_threshold_edit->setValidator(new QIntValidator(0, 255, this));
    m_ui->version_label->setText(QString("v") + _VERSION_);
}

SettingsForm::~SettingsForm()
{
    delete m_ui;
}

void SettingsForm::closeEvent(QCloseEvent *event)
{
    this->deleteLater();
    event->accept();
}

void SettingsForm::initConfig(){
    m_xml_controller.create();

    /*General*/
    m_xml_controller.append("Config.General.ClipFormat", "0");
    m_xml_controller.append("Config.General.SmartSpace", QString::number(SPACE_AI));

    /*Advance*/
    m_xml_controller.append("Config.Advance.LuminanceThreshold", "127");
    m_xml_controller.append("Config.Advance.MathpixID", EMPTY_STRING);
    m_xml_controller.append("Config.Advance.MathpixKEY", EMPTY_STRING);
    m_xml_controller.append("Config.Advance.OCRApiKEY", EMPTY_STRING);
    m_xml_controller.append("Config.Advance.OCRSecretKEY", EMPTY_STRING);
    m_xml_controller.append("Config.Advance.PromptLevel", "0");
    m_xml_controller.append("Config.Advance.PromptDetail", "0");

    /*HotKey*/
    m_xml_controller.append("Config.HotKey.ScreenShot", "Ctrl+Shift+A");
    m_xml_controller.append("Config.HotKey.Src", EMPTY_STRING);
    m_xml_controller.append("Config.HotKey.Clip", "Return");
    m_xml_controller.append("Config.HotKey.Cancel", "Esc");
    m_xml_controller.append("Config.HotKey.Save", "Ctrl+S");

    m_xml_controller.save();
}


void SettingsForm::loadConfig(){
    if (!m_xml_controller.load()) {
        qDebug() << "unable load config.xml";
        initConfig();
        m_xml_controller.load();
    }
    /*General*/
    QString val;
#if WIN32
    val = m_xml_controller.get("Config.General.ClipFormat");
    switch(val.toInt()){
    case WIN32_CF_BID: m_ui->xmind_radiobutton->setChecked(true); break;
    case WIN32_PNG: m_ui->office_radiobutton->setChecked(true); break;
    case WIN32_FILE: m_ui->png_radiobutton->setChecked(true); break;
    default: break;
    }
#elif __APPLE__
    m_ui->xmind_radiobutton->setEnabled(false);
    m_ui->office_radiobutton->setEnabled(false);
    m_ui->png_radiobutton->setEnabled(false);
#endif
    val = m_xml_controller.get("Config.General.SmartSpace");
    switch(val.toInt()){
    case SPACE_DISBALE: m_ui->space_disable_radioButton->setChecked(true); break;
    case SPACE_AI: m_ui->space_ai_radioButton->setChecked(true); break;
    case SPACE_IMITATE: m_ui->space_imitate_radioButton->setChecked(true); break;
    }

    /*Advance*/
    val = m_xml_controller.get("Config.Advance.LuminanceThreshold");
    m_ui->lu_threshold_edit->setText(val);
    m_ui->lu_threshold_slider->setValue(val.toInt());
    val = m_xml_controller.get("Config.Advance.MathpixID");
    m_ui->appid_lineEdit->setText(val == EMPTY_STRING ? "" : val);
    val = m_xml_controller.get("Config.Advance.MathpixKEY");
    m_ui->appkey_lineEdit->setText(val == EMPTY_STRING ? "" : val);
    val = m_xml_controller.get("Config.Advance.OCRApiKEY");
    m_ui->ocr_apikey_lineEdit->setText(val == EMPTY_STRING ? "" : val);
    val = m_xml_controller.get("Config.Advance.OCRSecretKEY");
    m_ui->ocr_secure_key_lineEdit->setText(val == EMPTY_STRING ? "" : val);
    val = m_xml_controller.get("Config.Advance.PromptLevel");
    m_ui->promptlevel_slider->setValue(val.toInt());
    val = m_xml_controller.get("Config.Advance.PromptDetail");
    m_ui->mathpix_checkBox->setChecked(val.toInt() & PROMPT_MATHPIX);
    /*HotKeys*/
    val = m_xml_controller.get("Config.HotKey.ScreenShot");
    m_ui->screenshot_edit->setKeySequence(QKeySequence(val));
    val = m_xml_controller.get("Config.HotKey.Src");
    m_ui->src_edit->setKeySequence(QKeySequence(val));
    val = m_xml_controller.get("Config.HotKey.Clip");
    m_ui->clip_edit->setKeySequence(QKeySequence(val));
    val = m_xml_controller.get("Config.HotKey.Save");
    m_ui->save_edit->setKeySequence(QKeySequence(val));
    val = m_xml_controller.get("Config.HotKey.Cancel");
    m_ui->cancel_edit->setKeySequence(QKeySequence(val));
}

void SettingsForm::save2File(){
    m_xml_controller.save();
    emit signalConfigSaved();
    this->close();
}

void SettingsForm::saveGeneralConfig(){
#if _WIN32
    int format = WIN32_CF_BID;
    if (m_ui->xmind_radiobutton->isChecked())format = WIN32_CF_BID;
    else if (m_ui->office_radiobutton->isChecked())format = WIN32_PNG;
    else if (m_ui->png_radiobutton->isChecked())format = WIN32_FILE;
    m_xml_controller.set("Config.General.ClipFormat", QString::number(format));
#endif
    if (m_ui->space_disable_radioButton->isChecked())format = SPACE_DISBALE;
    else if (m_ui->space_ai_radioButton->isChecked())format = SPACE_AI;
    else if (m_ui->space_imitate_radioButton->isChecked())format = SPACE_IMITATE;
    m_xml_controller.set("Config.General.SmartSpace", QString::number(format));
    save2File();
}

void SettingsForm::saveAdvanceConfig(){
    m_xml_controller.set("Config.Advance.LuminanceThreshold", m_ui->lu_threshold_edit->text());
    m_xml_controller.set("Config.Advance.MathpixID", m_ui->appid_lineEdit->text().isEmpty() ? EMPTY_STRING : m_ui->appid_lineEdit->text());
    m_xml_controller.set("Config.Advance.MathpixKEY", m_ui->appkey_lineEdit->text().isEmpty() ? EMPTY_STRING : m_ui->appkey_lineEdit->text());
    m_xml_controller.set("Config.Advance.OCRApiKEY", m_ui->ocr_apikey_lineEdit->text().isEmpty() ? EMPTY_STRING : m_ui->ocr_apikey_lineEdit->text());
    m_xml_controller.set("Config.Advance.OCRSecretKEY", m_ui->ocr_secure_key_lineEdit->text().isEmpty() ? EMPTY_STRING : m_ui->ocr_secure_key_lineEdit->text());
    m_xml_controller.set("Config.Advance.PromptLevel", QString::number(m_ui->promptlevel_slider->value()));
    short prompt_flag = 0;
    if (m_ui->mathpix_checkBox->isChecked())prompt_flag |= PROMPT_MATHPIX;
    m_xml_controller.set("Config.Advance.PromptDetail", QString::number(prompt_flag));
    save2File();
}

void SettingsForm::saveHotKeyConfig(){
    QString val = m_ui->screenshot_edit->keySequence().toString();
    m_xml_controller.set("Config.HotKey.ScreenShot", val == "" ? EMPTY_STRING : val);
    val = m_ui->src_edit->keySequence().toString();
    m_xml_controller.set("Config.HotKey.Src", val == "" ? EMPTY_STRING : val);
    val = m_ui->clip_edit->keySequence().toString();
    m_xml_controller.set("Config.HotKey.Clip", val == "" ? EMPTY_STRING : val);
    val = m_ui->cancel_edit->keySequence().toString();
    m_xml_controller.set("Config.HotKey.Cancel", val == "" ? EMPTY_STRING : val);
    val = m_ui->save_edit->keySequence().toString();
    m_xml_controller.set("Config.HotKey.Save", val == "" ? EMPTY_STRING : val);
    save2File();
}

void SettingsForm::on_hk_commit_button_clicked()
{
    saveHotKeyConfig();
}

void SettingsForm::on_hk_init_button_clicked()
{
    loadConfig();
}

void SettingsForm::on_hk_cancel_button_clicked()
{
    this->close();
}

void SettingsForm::on_adv_commit_button_clicked()
{
    saveAdvanceConfig();
}

void SettingsForm::on_adv_init_button_clicked()
{
    loadConfig();
}

void SettingsForm::on_adv_cancel_button_clicked()
{
    this->close();
}

void SettingsForm::on_gen_commit_button_clicked()
{
    saveGeneralConfig();
}

void SettingsForm::on_gen_init_button_clicked()
{
    loadConfig();
}

void SettingsForm::on_gen_cancel_button_clicked()
{
    this->close();
}

void SettingsForm::on_lu_threshold_slider_valueChanged(int value)
{
    m_ui->lu_threshold_edit->setText(QString::number(value));
}


void SettingsForm::on_lu_threshold_edit_textChanged(const QString &val)
{
    m_ui->lu_threshold_slider->setValue(val.toInt());
}

void SettingsForm::on_promptlevel_slider_valueChanged(int value)
{
    switch(value){
    case Lv_HINT:
        m_ui->promptlevel_label->setText("HINT");
        break;
    case Lv_WARNING:
        m_ui->promptlevel_label->setText("WARNING");
        break;
    case Lv_ERROR:
        m_ui->promptlevel_label->setText("ERROR");
        break;
    case Lv_NONE:
        m_ui->promptlevel_label->setText("NONE");
        break;
    }
}
