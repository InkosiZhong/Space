#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QClipboard>
#include <QMenu>
#include <QScreen>
#include <QFileDialog>
#include <QDateTime>
#include <QDomDocument>
#include <QTextStream>
#include <QMimeData>
#include <QThread>
#include "module/capturescreen.h"
#include "ui_mainwindow.h"
#include "qxtglobalshortcut.h"
#include "settingsform.h"
#include "promptform.h"
#include "exhibitform.h"
#include "moduledock.h"
#include "constants.h"
#include "utils/networkutil.h"
#include "module/smartspace.h"
// for clipboard
#if _WIN32
#include <Windows.h>
#include <atlimage.h>
#include <QBuffer>
#elif __APPLE__
#endif

#if _WIN32
extern enum HBitmapFormat hBitmapFormat;
extern enum ClipboardFormat clipboardFormat;
QT_BEGIN_NAMESPACE
Q_GUI_EXPORT HBITMAP qt_pixmapToWinHBITMAP(
    const QPixmap &p, int hbitmapFormat = 0);
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QThread dockThread;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void signalCompleteCature();
    void signalSetSrc(QPixmap*);
    void signalDockOperation(Operations op);

private:
    void initTrayIcon();
    void initConfig(); // call when the config.xml lost
    void initModules();
    void initHoverPrompt();
    void setupGeneralConfig();
    void setupAdvanceConfig();
    void setupHotKeyConfig();
    void screenCapture();
    void activeToolsDock(QPoint* ref_point);
    void activePromptDock(QPoint* ref_point);
    void onCaptureCompleted(bool save, bool origin = false);
    void onExitCapture();
    void copyImageToClipboard(QPixmap& image, int format = WIN32_CF_BID);
    bool eventFilter(QObject* obj, QEvent* event);
    void setFunctionActive(int op);

private slots:
    void onTrayActive(QSystemTrayIcon::ActivationReason reason);
    void onFinishCapture(DataPackage* data);
    void activeSettingsWindow();
    void exitProgram();
    void loadConfig();
    void onReceiveMap(QPixmap* map);
    void onReceiveFormula(LatexInfoPack* latex);
    void onReceiveOCR(OCRInfoPack* ocr);
    void onExitOCRExhibit();
    void onSetFunctionActive(bool);

    void on_copy_button_clicked();
    void on_exit_button_clicked();
    void on_save_button_clicked();
    void on_source_button_clicked();
    void on_setting_button_clicked();
    void on_latex_button_clicked();

    void on_ocr_button_clicked();

    void on_smart_space_pushButton_clicked();

private:
    Ui::MainWindow *m_ui;
#if _WIN32
    Util::XmlUtil m_xml_controller = Util::XmlUtil("config.xml", ".");
#elif __APPLE__
    Util::XmlUtil m_xml_controller = Util::XmlUtil(qApp->applicationDirPath() + "/config.xml", ".");
#endif
    SettingsForm *m_settings = NULL;
    PromptForm* m_prompt = NULL;
    ExhibitForm* m_exhibit = NULL;
    CaptureScreen* m_capturer = NULL;
    ModuleDock* m_module_dock;
    QMenu* m_tray_menu;
    bool m_capturing = false;
    QxtGlobalShortcut* m_shortcut;
    int m_format = WIN32_CF_BID;
    bool m_save = false, m_origin = false;
    Util::NetworkUtil* m_net_util = NULL;
    SmartSpace m_smart_space;
};
#endif // MAINWINDOW_H
