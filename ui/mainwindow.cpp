#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    m_shortcut = new QxtGlobalShortcut(this);
    m_module_dock = new ModuleDock;
    m_prompt = new PromptForm();
    loadConfig();
    initTrayIcon();
    initModules();
    initHoverPrompt();
}

MainWindow::~MainWindow()
{
    dockThread.quit();
    dockThread.wait();
    spaceThread.quit();
    spaceThread.wait();
    this->setParent(NULL);
    delete m_ui;
}

void MainWindow::initTrayIcon(){
    // icon
    QSystemTrayIcon* systemTray = new QSystemTrayIcon(this);
    systemTray->setIcon(QIcon(":/icon/res/icon.png"));
    systemTray->setToolTip("Space");
    systemTray->show();

    // menu
    m_tray_menu = new QMenu(this);
    QAction* settingAction = new QAction(m_tray_menu);
    QAction* exitAction = new QAction(m_tray_menu);
    settingAction->setText("Settings");
    exitAction->setText("Exit");
    m_tray_menu->addAction(settingAction);
    m_tray_menu->addAction(exitAction);
    connect(settingAction, &QAction::triggered, this, &MainWindow::activeSettingsWindow);
    connect(exitAction, &QAction::triggered, this, &MainWindow::exitProgram);
    systemTray->setContextMenu(m_tray_menu);

    connect(systemTray, &QSystemTrayIcon::activated, this, &MainWindow::onTrayActive);
}

void MainWindow::initConfig(){
    m_xml_controller.create();

    /*General*/
    m_xml_controller.append("Config.General.ClipFormat", QString::number(WIN32_CF_BID));
    m_xml_controller.append("Config.General.SmartSpace", QString::number(SPACE_AI));
    m_xml_controller.append("Config.General.Performance", QString::number(PERFORM_LIGHTWEIGHT));

    /*Advance*/
    m_xml_controller.append("Config.Advaznce.LuminanceThreshold", "127");
    m_xml_controller.append("Config.Advance.MathpixID", "");
    m_xml_controller.append("Config.Advance.MathpixKEY", "");
    m_xml_controller.append("Config.Advance.OCRApiKEY", "");
    m_xml_controller.append("Config.Advance.OCRSecretKEY", "");
    m_xml_controller.append("Config.Advance.PromptLevel", QString::number(Lv_HINT));
    m_xml_controller.append("Config.Advance.PromptDetail", "0");

    /*HotKey*/
    m_xml_controller.append("Config.HotKey.ScreenShot", "Ctrl+Shift+A");
    m_xml_controller.append("Config.HotKey.Src", "");
    m_xml_controller.append("Config.HotKey.Clip", "Return");
    m_xml_controller.append("Config.HotKey.Cancel", "Esc");
    m_xml_controller.append("Config.HotKey.Save", "Ctrl+S");
    m_xml_controller.append("Config.HotKey.LaTeX", "");
    m_xml_controller.append("Config.HotKey.OCR", "");

    m_xml_controller.save();
}

void enableButtonHover(QPushButton* btn, MainWindow* it){
    btn->setAttribute(Qt::WA_Hover, true);
    btn->installEventFilter(it);
}

void MainWindow::initHoverPrompt(){
    enableButtonHover(m_ui->latex_button, this);
    enableButtonHover(m_ui->ocr_button, this);
    enableButtonHover(m_ui->code_button, this);
    enableButtonHover(m_ui->concat_button, this);
    enableButtonHover(m_ui->setting_button, this);
    enableButtonHover(m_ui->source_button, this);
}

void setBtnStyle(QPushButton* btn, QString txt, bool highlight = false){
    if(btn->isEnabled()){
        QString style;
        if (highlight){
            style = "QPushButton{border-image: url(:/icon/res/btn_icon/highlight/" + txt + ".png)}";
        } else {
            style = "QPushButton{border-image: url(:/icon/res/btn_icon/default/" + txt + ".png)}";
        }
        style = style + "QPushButton:hover{border-image: url(:/icon/res/btn_icon/hover/" + txt + ".png)}" +
                "QPushButton:pressed{border-image: url(:/icon/res/btn_icon/press/" + txt + ".png)}" +
                "QPushButton:!enabled{border-image: url(:/icon/res/btn_icon/unable/" + txt + ".png)}";
        btn->setStyleSheet(style);
    }
}

void MainWindow::onSetHighLight(bool active){
    if (active){
        const std::list<Operations> infer = m_smart_space.inference();
        if (infer.empty())return;
        switch (infer.front()){
        case GetSrc: setBtnStyle(m_ui->source_button, "picture", true); break;
        case GetOperated: setBtnStyle(m_ui->copy_button, "copy", true); break;
        case GetFormula: setBtnStyle(m_ui->latex_button, "LaTex", true); break;
        case GetOCR: setBtnStyle(m_ui->ocr_button, "OCR", true); break;
        default: break;
        }
    } else {
        setBtnStyle(m_ui->source_button, "picture", false);
        setBtnStyle(m_ui->copy_button, "copy", false);
        setBtnStyle(m_ui->latex_button, "LaTex", false);
        setBtnStyle(m_ui->ocr_button, "OCR", false);
    }
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event){
    if (event->type() == QEvent::HoverEnter){
        if (obj == m_ui->latex_button){
            QString prompt = "extracting LaTeX formula";
            if (!m_ui->latex_button->isEnabled())prompt += " (activate in setting)";
            m_prompt->setPrompt(prompt, Lv_HINT, this);
            setBtnStyle(m_ui->latex_button, "LaTex");
        }
        if (obj == m_ui->ocr_button){
            if (!m_ui->ocr_button->isEnabled()){
                m_prompt->setPrompt("OCR (activate in setting)", Lv_HINT, this);
            }
            setBtnStyle(m_ui->ocr_button, "OCR");
        }
        else if (obj == m_ui->code_button){
            m_prompt->setPrompt("extracting Code", Lv_HINT, this);
            setBtnStyle(m_ui->code_button, "code");
        }
        else if (obj == m_ui->concat_button){
            m_prompt->setPrompt("concatenating images", Lv_HINT, this);
            setBtnStyle(m_ui->concat_button, "layout");
        }
        else if (obj == m_ui->setting_button){
            m_prompt->setPrompt("customizing settings", Lv_HINT, this);
            setBtnStyle(m_ui->setting_button, "adjust");
        }
        else if (obj == m_ui->source_button){
            m_prompt->setPrompt("get source images", Lv_HINT, this);
            setBtnStyle(m_ui->source_button, "picture");
        }
        return true;
    }
    else if(event->type() == QEvent::HoverLeave){
        if (obj == m_ui->latex_button || obj == m_ui->setting_button || obj == m_ui->source_button ||
            obj == m_ui->concat_button || obj == m_ui->ocr_button){
            m_prompt->hidePrompt();
            setBtnStyle(m_ui->latex_button, "LaTex");
            setBtnStyle(m_ui->ocr_button, "OCR");
            setBtnStyle(m_ui->code_button, "code");
            setBtnStyle(m_ui->concat_button, "layout");
            setBtnStyle(m_ui->setting_button, "adjust");
            setBtnStyle(m_ui->source_button, "picture");
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void MainWindow::loadConfig(){
    m_xml_controller.addMapping("", EMPTY_STRING);
    m_xml_controller.addMapping(EMPTY_STRING, "");
    if (!m_xml_controller.load()){
        qDebug() << "unable load config.xml";
        initConfig();
        m_xml_controller.load();
    }
    setupGeneralConfig();
    setupAdvanceConfig();
    setupHotKeyConfig();
}

void MainWindow::setupGeneralConfig(){
    QString val = m_xml_controller.get("Config.General.ClipFormat");
    m_format = val.toInt();
    val = m_xml_controller.get("Config.General.SmartSpace");
    m_smart_space.setMode(SmartSpaceMode(val.toInt()));
    val = m_xml_controller.get("Config.General.Performance");
    m_perform_level = Performance(val.toInt());
}

void MainWindow::setupAdvanceConfig(){
    QString lu = m_xml_controller.get("Config.Advance.LuminanceThreshold");
    QString mathpix_id = m_xml_controller.get("Config.Advance.MathpixID");
    QString mathpix_key = m_xml_controller.get("Config.Advance.MathpixKEY");
    QString ocr_api_key = m_xml_controller.get("Config.Advance.OCRApiKEY");
    QString ocr_sec_key = m_xml_controller.get("Config.Advance.OCRSecretKEY");
    ConfigPack cfg = ConfigPack(m_perform_level, lu.toInt(), mathpix_id, mathpix_key, ocr_api_key, ocr_sec_key);
    m_module_dock->setup(cfg);
    QString val = m_xml_controller.get("Config.Advance.PromptLevel");
    m_prompt->setMinLevel(val.toInt());
    val = m_xml_controller.get("Config.Advance.PromptDetail");
}

void MainWindow::setupHotKeyConfig(){
    QString val = m_xml_controller.get("Config.HotKey.ScreenShot");
    disconnect(m_shortcut, NULL, NULL, NULL);
    m_shortcut->setShortcut(QKeySequence(val));
    connect(m_shortcut, &QxtGlobalShortcut::activated,
        [=]() {screenCapture();});

    m_ui->source_button->setShortcut(QKeySequence(m_xml_controller.get("Config.HotKey.Src")));
    m_ui->copy_button->setShortcut(QKeySequence(m_xml_controller.get("Config.HotKey.Clip")));
    m_ui->exit_button->setShortcut(QKeySequence(m_xml_controller.get("Config.HotKey.Cancel")));
    m_ui->save_button->setShortcut(QKeySequence(m_xml_controller.get("Config.HotKey.Save")));
    m_ui->latex_button->setShortcut(QKeySequence(m_xml_controller.get("Config.HotKey.LaTeX")));
    m_ui->ocr_button->setShortcut(QKeySequence(m_xml_controller.get("Config.HotKey.OCR")));
}

void MainWindow::initModules(){
    m_module_dock->moveToThread(&dockThread);
    connect(this, SIGNAL(signalSetSrc(QPixmap*)), m_module_dock, SLOT(onSetSrc(QPixmap*)));
    connect(&dockThread, &QThread::finished, m_module_dock, &QObject::deleteLater);
    qRegisterMetaType<Operations>("Operations");
    connect(this, SIGNAL(signalDockOperation(Operations)), m_module_dock, SLOT(onOperation(Operations)));
    connect(m_module_dock, SIGNAL(signalSendMap(QPixmap*)), this, SLOT(onReceiveMap(QPixmap*)));
    connect(m_module_dock, SIGNAL(signalSendFormula(LatexInfoPack*)), this, SLOT(onReceiveFormula(LatexInfoPack*)));
    connect(m_module_dock, SIGNAL(signalSendOCR(OCRInfoPack*)), this, SLOT(onReceiveOCR(OCRInfoPack*)));
    dockThread.start();

    m_smart_space.setModuleDock(m_module_dock);
    m_smart_space.moveToThread(&spaceThread);
    connect(this, SIGNAL(signalSetSrc(QPixmap*)), &m_smart_space, SLOT(AIInference(QPixmap*)));
    connect(this, SIGNAL(signalDockOperation(Operations)), &m_smart_space, SLOT(onOperation(Operations)));
    connect(&m_smart_space, SIGNAL(signalState(bool)), this, SLOT(onSetHighLight(bool)));
    spaceThread.start();
}

void MainWindow::screenCapture(){
    if (m_capturing)return;
    m_capturing = true;
    this->setParent(NULL);
    if (!m_capturer || m_perform_level < PERFORM_SPEED){
        m_capturer = new CaptureScreen();
    } else {
        m_capturer->active();
    }
    this->setParent(m_capturer);
    connect(m_capturer, SIGNAL(signalFinishCapture(DataPackage*)), this, SLOT(onFinishCapture(DataPackage*)));
    connect(m_capturer, SIGNAL(signalCancelCapture()), this, SLOT(on_exit_button_clicked()));
    connect(this, SIGNAL(signalCompleteCature()), m_capturer, SLOT(onCaptureCompleted()));
    m_capturer->show();
}

void MainWindow::exitProgram(){
    this->setParent(NULL);
    this->close();
    QApplication::quit();
}

void MainWindow::activeSettingsWindow(){
    m_settings = new SettingsForm();
    connect(m_settings, SIGNAL(signalConfigSaved()), this, SLOT(loadConfig()));
    m_settings->show();
    m_settings->raise();
}

void MainWindow::setFunctionActive(int op){
    bool active = m_module_dock->operationAvailable(op) && m_net_util->isConnecting();
    switch(op){
    case GetOCR:
        m_ui->ocr_button->setEnabled(active);
        break;
    case GetFormula:
        m_ui->latex_button->setEnabled(active);
        break;
    default:
        break;
    }
}

void MainWindow::onSetFunctionActive(bool){
    setFunctionActive(GetOCR);
    setFunctionActive(GetFormula);
}

void MainWindow::copyImageToClipboard(QPixmap& image, int format){
#if _WIN32
    HWND hwnd;
    hwnd = (HWND)this->winId();
    switch(format){
    case WIN32_CF_BID: {
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        ULONG_PTR gdiplusToken;
        GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

        HBITMAP hbitmap = qt_pixmapToWinHBITMAP(image, HBitmapAlpha);
        BITMAP bm;
        GetObject(hbitmap, sizeof bm, &bm);

        BITMAPINFOHEADER bi = { sizeof bi, bm.bmWidth, bm.bmHeight, 1, bm.bmBitsPixel, BI_RGB };
        std::vector<BYTE> vec(bm.bmWidthBytes * bm.bmHeight);
        auto hdc = GetDC(NULL);
        GetDIBits(hdc, hbitmap, 0, bi.biHeight, vec.data(), (BITMAPINFO*)&bi, 0);
        ReleaseDC(NULL, hdc);

        auto hmem = GlobalAlloc(GMEM_MOVEABLE, sizeof bi + vec.size());
        auto buffer = (BYTE*)GlobalLock(hmem);
        memcpy(buffer, &bi, sizeof bi);
        memcpy(buffer + sizeof bi, vec.data(), vec.size());
        GlobalUnlock(hmem);

        if(OpenClipboard(hwnd))
        {
            EmptyClipboard();
            SetClipboardData(CF_DIB, hmem);
            CloseClipboard();
        }
        DeleteObject(hbitmap);
        Gdiplus::GdiplusShutdown(gdiplusToken);
        break;
    }
    case WIN32_PNG: {
        image.save("temp.png");
        // Get the file size
        struct stat st;
        const char* const filename = "temp.png";
        stat(filename, &st);
        // Allocate buffer and read data
        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, st.st_size);
        LPVOID pGlobal = GlobalLock(hGlobal);
        FILE *f = fopen(filename, "rb");
        fread(pGlobal, 1, st.st_size, f);
        fclose(f);
        GlobalUnlock(hGlobal);
        // Register clipboard format
        UINT cf = RegisterClipboardFormat(L"PNG");
        // Put it on the clipboard.
        // The global memory is owned by the clipboard and will be freed when
        //  new data is put on the clipboard.
        OpenClipboard(hwnd);
        EmptyClipboard();
        SetClipboardData(cf, hGlobal);
        CloseClipboard();
        break;
    }
    case WIN32_FILE: {
        image.save("temp.png");
        QClipboard *clip = QApplication::clipboard();
        QList<QUrl> copyfile;
        QUrl url=QUrl::fromLocalFile(QDir::currentPath() + "/temp.png");    //待复制的文件
        if(url.isValid()){
            copyfile.push_back(url);
        }else{
            return;
        }
        QMimeData* mimeData = new QMimeData;
        mimeData->setUrls(copyfile);
        clip->setMimeData(mimeData);
        break;
    }
    default: {
        QClipboard *clip = QApplication::clipboard();
        clip->setPixmap(image);
        break;
    }
    }
#elif __APPLE__
    QClipboard *clip = QApplication::clipboard();
    clip->setPixmap(image);
#endif
}

void MainWindow::onReceiveMap(QPixmap* map){
    m_prompt->hidePrompt();
    if (!map)return;
    if (!map->isNull()){
        if (m_save){ // save to disk
            QDateTime current_date_time = QDateTime::currentDateTime();
            QString filename = "./Space_" + current_date_time.toString("yyyyMMdd_hh-mm-ss") + ".png";
            filename = QFileDialog::getSaveFileName(this,tr("Save Image"),filename,tr("Images (*.png *.bmp *.jpg)"));
            if (filename.isEmpty() || filename.isNull())return;
            map->save(filename);
        }
        else { // save to clipboard
            copyImageToClipboard(*map, m_origin ? -1 : m_format);
        }
    }
    map = NULL;
    emit signalCompleteCature();
    onExitCapture();
}

void MainWindow::onReceiveFormula(LatexInfoPack* latex){
    if (!latex)return;
    if (latex->m_confidence > 0.2){
        QClipboard *clip = QApplication::clipboard();
        clip->setText(latex->m_msg);
        emit signalCompleteCature();
        onExitCapture();
    }
    else {
        if (m_xml_controller.get("Config.Advance.PromptDetail").toInt()){
            m_prompt->setPrompt(latex->m_msg+ " (Mathpix)", Lv_ERROR, this);
        }
        else{
            if (m_net_util->isConnecting()){
                m_prompt->setPrompt("Failed to detect formula in the area!", Lv_WARNING, this);
            } else {
                m_prompt->setPrompt("check your network connection!", Lv_ERROR, this);
            }
        }
    }
}

void MainWindow::onReceiveOCR(OCRInfoPack* ocr){
    if (!ocr)return;
    if (ocr->ocr_result.empty()){
        if (m_net_util->isConnecting()){
            m_prompt->setPrompt("OCR return nothing!", Lv_WARNING, this);
        } else {
            m_prompt->setPrompt("check your network connection!", Lv_ERROR, this);
        }
        return;
    }
    //emit signalCompleteCature();
    if (ocr->ocr_result.size() == 1){
        QClipboard *clip = QApplication::clipboard();
        clip->setText(ocr->ocr_result.front().words);
    } else {
        if (m_exhibit){
            m_exhibit->close();
            m_exhibit = NULL;
        }
        m_exhibit = new ExhibitForm(ocr);
        m_exhibit->show();
        connect(m_exhibit, SIGNAL(signalFinishExhibit()), this, SLOT(onExitOCRExhibit()));
    }
    onExitCapture();
}

void MainWindow::onCaptureCompleted(bool save, bool origin){
    m_save = save;
    m_origin = origin;
    emit signalDockOperation(origin ? GetSrc : GetOperated);
    m_prompt->setPrompt("wait for a while ...", Lv_HINT, this);
}

void MainWindow::onFinishCapture(DataPackage* data){
    if (!m_net_util){
        m_net_util = new Util::NetworkUtil();
        connect(m_net_util, SIGNAL(signalConnStateChanged(bool)), this, SLOT(onSetFunctionActive(bool)));
    }
    m_prompt->hidePrompt();
    onSetHighLight(true);
    if (data){
        QRect screen = QGuiApplication::screens().at(data->screen_id)->geometry();
        QPoint ref_point = *data->ref_point + screen.topLeft();
        if (data->corner & LEFT)ref_point.setX(std::min(ref_point.x(), screen.x() + screen.width() - this->width()));   // LEFT
        else ref_point.setX(std::max(ref_point.x() - this->width(), screen.x()));                                       // RIGHT
        if (data->corner & TOP)ref_point.setY(std::max(ref_point.y() - this->height() - 8, screen.y()));                // TOP
        else ref_point.setY(std::min(ref_point.y() + 8, screen.y() + screen.height() - this->height()));                // BOTTOM
        activeToolsDock(&ref_point);

        ref_point.setY(ref_point.y() + this->height());
        activePromptDock(&ref_point);

        if (data->src){
            emit signalSetSrc(data->src);
        }
    }
    else activeToolsDock(NULL);
}

void MainWindow::onExitOCRExhibit(){
    if (m_exhibit){
        disconnect(m_exhibit, SIGNAL(signalFinishExhibit()), this, SLOT(onExitOCRExhibit()));
        m_exhibit = NULL;
    }
}

void MainWindow::onExitCapture(){
    this->setParent(NULL);
    activeToolsDock(NULL);
    m_prompt->hidePrompt();
    m_capturing = false;
    if (m_capturer && m_perform_level < PERFORM_SPEED){
        m_capturer->close();
        m_capturer = NULL;
    }
    if (m_net_util){
        m_net_util->deleteLater();
        m_net_util = NULL;
    }
    emit signalDockOperation(Clean);
}

void MainWindow::onTrayActive(QSystemTrayIcon::ActivationReason reason){
    switch (reason){
    case QSystemTrayIcon::Context:
#if __WIN32
        m_tray_menu->show();
#endif
        break;
    case QSystemTrayIcon::DoubleClick:
        break;
    case QSystemTrayIcon::Trigger:
        screenCapture();
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    case QSystemTrayIcon::Unknown:
        break;
    }
}

void MainWindow::activeToolsDock(QPoint* ref_point){
    Qt::WindowFlags m_flags = windowFlags();
    setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
    if (ref_point)move(*ref_point), show();
    else hide();
}

void MainWindow::activePromptDock(QPoint* ref_point){
    m_prompt->move(*ref_point);
}

void MainWindow::on_copy_button_clicked()
{
    onCaptureCompleted(false);
}

void MainWindow::on_exit_button_clicked()
{
    emit signalCompleteCature();
    onExitCapture();
}

void MainWindow::on_save_button_clicked()
{
    onCaptureCompleted(true);
}

void MainWindow::on_source_button_clicked()
{
    onCaptureCompleted(false, true);
}

void MainWindow::on_setting_button_clicked()
{
    activeSettingsWindow();
    loadConfig();
}


void MainWindow::on_latex_button_clicked()
{
    emit signalDockOperation(GetFormula);
    m_prompt->setPrompt("wait for a while ...", Lv_HINT, this);
}


void MainWindow::on_ocr_button_clicked()
{
    emit signalDockOperation(GetOCR);
    m_prompt->setPrompt("wait for a while ...", Lv_HINT, this);
}


void MainWindow::on_smart_space_pushButton_clicked()
{
    m_smart_space.accept();
}
