#include "capturescreen.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QScreen>


CaptureScreen::CaptureScreen(QWidget *parent)
    : QWidget(parent)
    //, m_is_mouse_press(false)
{
    initWindow();
    active();
}

CaptureScreen::~CaptureScreen() {}

void CaptureScreen::active(){
    m_begin_point = QPoint();
    m_end_point = QPoint();
    m_begin_move_point = QPoint();
    m_end_move_point = QPoint();
    initStretchRect();
    loadBackgroundPixmap();
#if __APPLE__
    setFixedSize(m_screen_width, m_screen_height);
#endif
}

void CaptureScreen::initWindow()
{
    this->setMouseTracking(true);
#if _WIN32
    this->setWindowFlags(Qt::FramelessWindowHint);
    setWindowState(Qt::WindowActive | Qt::WindowFullScreen);
#elif __APPLE__
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint | Qt::WindowStaysOnTopHint);
#endif
}

void CaptureScreen::initStretchRect()
{
    m_stretchRectState = NotSelect;
    m_top_left_rect = QRect(0 , 0 , 0 , 0);
    m_top_right_rect = QRect(0, 0, 0, 0);
    m_bottom_left_rect = QRect(0, 0, 0, 0);
    m_bottom_right_rect = QRect(0, 0, 0, 0);

    m_left_center_rect = QRect(0, 0, 0, 0);
    m_top_center_rect = QRect(0, 0, 0, 0);
    m_right_center_rect = QRect(0, 0, 0, 0);
    m_bottom_center_rect = QRect(0, 0, 0, 0);
}

void CaptureScreen::loadBackgroundPixmap()
{
    int screen_id = QApplication::desktop()->screenNumber(this);  //抓取当前屏幕的图片;
    QScreen *screen = QGuiApplication::screens().at(screen_id);
    m_load_pixmap = screen->grabWindow(0);
    m_screen_width = m_load_pixmap.width();
    m_screen_height = m_load_pixmap.height();
    m_data.screen_id = screen_id;
}

void CaptureScreen::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        emit signalCancelCapture();
    }
}

void CaptureScreen::mousePressEvent(QMouseEvent *event)
{
    m_stretchRectState = getStrethRectState(event->pos());
    if (event->button() == Qt::LeftButton)
    {
        if (m_current_capture_state == InitCapture)
        {
            m_current_capture_state = BeginCaptureImage;
            m_begin_point = m_end_point = event->pos();
        }
        // 是否在拉伸的小矩形中;
        else if (m_stretchRectState != NotSelect)
        {
            m_current_capture_state = BeginMoveStretchRect;
            // 当前鼠标在拖动选中区顶点时,设置鼠标当前状态;
            setStretchCursorStyle(m_stretchRectState);
            m_begin_move_point = m_end_move_point = event->pos();
        }
        // 是否在选中的矩形中;
        else if (isPressPointInSelectRect(event->pos()))
        {
            m_current_capture_state = BeginMoveCaptureArea;
            m_begin_move_point = m_end_move_point = event->pos();
        }
        else if (m_current_capture_state == FinishCaptureImage ||
                 m_current_capture_state == FinishMoveCaptureArea ||
                 m_current_capture_state == FinishMoveStretchRect){
            m_current_capture_state = BeginCaptureImage;
            m_begin_point = m_end_point = event->pos();
        }
        else { return QWidget::mousePressEvent(event); }
        emit signalFinishCapture(NULL);
    }

    return QWidget::mousePressEvent(event);
}

void CaptureScreen::mouseMoveEvent(QMouseEvent* event)
{
    if (m_current_capture_state == BeginCaptureImage)
    {
        m_end_point = event->pos();
        update();
    }
    else if (m_current_capture_state == BeginMoveCaptureArea)
    {
        m_end_move_point = event->pos();
        update();
    }
    else if (m_current_capture_state == BeginMoveStretchRect)
    {
        m_end_move_point = event->pos();
        update();
        return QWidget::mouseMoveEvent(event);
    }

    StretchRectState stretchRectState = getStrethRectState(event->pos());
    if (stretchRectState != NotSelect)
    {
        setStretchCursorStyle(stretchRectState);
    }
    else if (isPressPointInSelectRect(event->pos()))
    {
        setCursor(Qt::SizeAllCursor);
    }
    else if (!isPressPointInSelectRect(event->pos()) && m_current_capture_state != BeginMoveCaptureArea)
    {
        setCursor(Qt::ArrowCursor);
    }

    return QWidget::mouseMoveEvent(event);
}

void CaptureScreen::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        m_data.corner = 0;
        if (m_current_capture_state == BeginCaptureImage)
        {
            m_current_capture_state = FinishCaptureImage;
            m_end_point = event->pos();
            m_data.src = &m_capture_pixmap;
            m_data.ref_point = &m_end_point;
            m_end_point.x() < m_begin_point.x() ? m_data.corner |= LEFT : RIGHT;
            m_end_point.y() < m_begin_point.y() ? m_data.corner |= TOP : BOTTOM;
        }
        else if (m_current_capture_state == BeginMoveCaptureArea)
        {
            m_current_capture_state = FinishMoveCaptureArea;
            m_end_move_point = event->pos();
            m_data.src = &m_capture_pixmap;
            m_data.ref_point = &m_end_move_point;
            m_end_move_point.x() < m_begin_point.x() ? m_data.corner |= LEFT : RIGHT;
            m_end_move_point.y() < m_begin_point.y() ? m_data.corner |= TOP : BOTTOM;
        }
        else if (m_current_capture_state == BeginMoveStretchRect)
        {
            m_current_capture_state = FinishMoveStretchRect;
            m_end_move_point = event->pos();
            m_data.src = &m_capture_pixmap;
            m_data.ref_point = &m_end_move_point;
            m_end_move_point.x() < m_begin_point.x() ? m_data.corner |= LEFT : RIGHT;
            m_end_move_point.y() < m_begin_point.y() ? m_data.corner |= TOP : BOTTOM;
        }
        else { return QWidget::mouseReleaseEvent(event); }
        emit signalFinishCapture(&m_data);
        update();
    }
    return QWidget::mouseReleaseEvent(event);
}

void CaptureScreen::closeEvent(QCloseEvent *event){
    this->deleteLater();
    event->accept();
}

bool CaptureScreen::isPressPointInSelectRect(QPoint mousePressPoint)
{
    QRect selectRect = getRect(m_begin_point, m_end_point);
    return selectRect.contains(mousePressPoint);
}

void CaptureScreen::paintEvent(QPaintEvent *event)
{
    m_painter.begin(this);          //进行重绘;

    m_painter.setPen(QPen(color_accent, 3, Qt::SolidLine, Qt::FlatCap));    //设置画笔;
    m_painter.drawPixmap(0, 0, m_load_pixmap);                       //将背景图片画到窗体上;
    m_painter.fillRect(m_load_pixmap.rect(), color_shadow);           //画影罩效果;

    switch (m_current_capture_state)
    {
    case InitCapture:
        break;
    case BeginCaptureImage:
    case FinishCaptureImage:
    case BeginMoveCaptureArea:
    case FinishMoveCaptureArea:
    case BeginMoveStretchRect:
    case FinishMoveStretchRect:
        m_current_select_rect = getSelectRect();
        drawCaptureImage();
    default:
        break;
    }

    m_painter.end();  //重绘结束;
}

void CaptureScreen::onCaptureCompleted(){
    this->hide();
}

QRect CaptureScreen::getSelectRect()
{
    if (m_current_capture_state == BeginCaptureImage || m_current_capture_state == FinishCaptureImage)
    {
        return getRect(m_begin_point, m_end_point);
    }
    else if (m_current_capture_state == BeginMoveCaptureArea || m_current_capture_state == FinishMoveCaptureArea)
    {
        return getMoveRect();
    }
    else if (m_current_capture_state == BeginMoveStretchRect || m_current_capture_state == FinishMoveStretchRect)
    {
        return getStretchRect();
    }

    return QRect(0, 0, 0, 0);
}

void CaptureScreen::drawCaptureImage()
{
    float scale = Util::Util::windowsDpiScale();
    m_capture_pixmap = m_load_pixmap.copy(getRect(m_current_select_rect.topLeft()*scale,
                                                  m_current_select_rect.bottomRight()*scale));
    m_painter.drawPixmap(m_current_select_rect.topLeft(), m_capture_pixmap);
    m_painter.drawRect(m_current_select_rect);
    drawStretchRect();
}

void CaptureScreen::drawStretchRect()
{
    // 四个角坐标;
    QPoint topLeft = m_current_select_rect.topLeft();
    QPoint topRight = m_current_select_rect.topRight();
    QPoint bottomLeft = m_current_select_rect.bottomLeft();
    QPoint bottomRight = m_current_select_rect.bottomRight();
    // 四条边中间点坐标;
    QPoint leftCenter = QPoint(topLeft.x(), (topLeft.y() + bottomLeft.y()) / 2);
    QPoint topCenter = QPoint((topLeft.x() + topRight.x()) / 2, topLeft.y());
    QPoint rightCenter = QPoint(topRight.x(), leftCenter.y());
    QPoint bottomCenter = QPoint(topCenter.x(), bottomLeft.y());

    m_top_left_rect = QRect(topLeft.x() - STRETCH_DIAMETER / 2, topLeft.y() - STRETCH_DIAMETER / 2, STRETCH_DIAMETER, STRETCH_DIAMETER);
    m_top_right_rect = QRect(topRight.x() - STRETCH_DIAMETER / 2, topRight.y() - STRETCH_DIAMETER / 2, STRETCH_DIAMETER, STRETCH_DIAMETER);
    m_bottom_left_rect = QRect(bottomLeft.x() - STRETCH_DIAMETER / 2, bottomLeft.y() - STRETCH_DIAMETER / 2, STRETCH_DIAMETER, STRETCH_DIAMETER);
    m_bottom_right_rect = QRect(bottomRight.x() - STRETCH_DIAMETER / 2, bottomRight.y() - STRETCH_DIAMETER / 2, STRETCH_DIAMETER, STRETCH_DIAMETER);

    m_left_center_rect = QRect(leftCenter.x() - STRETCH_DIAMETER / 2, leftCenter.y() - STRETCH_DIAMETER / 2, STRETCH_DIAMETER, STRETCH_DIAMETER);
    m_top_center_rect = QRect(topCenter.x() - STRETCH_DIAMETER / 2, topCenter.y() - STRETCH_DIAMETER / 2, STRETCH_DIAMETER, STRETCH_DIAMETER);
    m_right_center_rect = QRect(rightCenter.x() - STRETCH_DIAMETER / 2, rightCenter.y() - STRETCH_DIAMETER / 2, STRETCH_DIAMETER, STRETCH_DIAMETER);
    m_bottom_center_rect = QRect(bottomCenter.x() - STRETCH_DIAMETER / 2, bottomCenter.y() - STRETCH_DIAMETER / 2, STRETCH_DIAMETER, STRETCH_DIAMETER);

    m_painter.setPen(QPen(color_bright_gray, 1.2, Qt::SolidLine));
    m_painter.setBrush(QBrush(color_accent));
    m_painter.drawEllipse(m_top_left_rect);
    m_painter.drawEllipse(m_top_right_rect);
    m_painter.drawEllipse(m_bottom_left_rect);
    m_painter.drawEllipse(m_bottom_right_rect);
    m_painter.drawEllipse(m_left_center_rect);
    m_painter.drawEllipse(m_top_center_rect);
    m_painter.drawEllipse(m_right_center_rect);
    m_painter.drawEllipse(m_bottom_center_rect);
}

QRect CaptureScreen::getRect(const QPoint &beginPoint, const QPoint &endPoint)
{
    int x, y, width, height;
    width = qAbs(beginPoint.x() - endPoint.x());
    height = qAbs(beginPoint.y() - endPoint.y());
    x = beginPoint.x() < endPoint.x() ? beginPoint.x() : endPoint.x();
    y = beginPoint.y() < endPoint.y() ? beginPoint.y() : endPoint.y();

    QRect selectedRect = QRect(x, y, width, height);
    if (selectedRect.width() == 0)
        selectedRect.setWidth(1);
    if (selectedRect.height() == 0)
        selectedRect.setHeight(1);

    return selectedRect;
}

QRect CaptureScreen::getMoveRect()
{
    // 通过getMovePoint方法先检查当前是否移动超出屏幕;
    QPoint move_point = getMovePoint();
    QPoint begin_point = m_begin_point + move_point;
    QPoint end_point = m_end_point + move_point;
    // 结束移动选区时更新当前m_begin_point , m_end_point,防止下一次操作时截取的图片有问题;
    if (m_current_capture_state == FinishMoveCaptureArea)
    {
        m_begin_point = begin_point;
        m_end_point = end_point;
        m_begin_move_point = QPoint(0, 0);
        m_end_move_point = QPoint(0, 0);
    }
    return getRect(begin_point, end_point);
}

QPoint CaptureScreen::getMovePoint()
{
    QPoint movePoint = m_end_move_point - m_begin_move_point;
    QRect currentRect = getRect(m_begin_point, m_end_point);
    // 检查当前是否移动超出屏幕;

    //移动选区是否超出屏幕左边界;
    if (currentRect.topLeft().x() + movePoint.x() < 0)
    {
        movePoint.setX(0 - currentRect.topLeft().x());
    }
    //移动选区是否超出屏幕上边界;
    if (currentRect.topLeft().y() + movePoint.y() < 0)
    {
        movePoint.setY(0 - currentRect.topLeft().y());
    }
    //移动选区是否超出屏幕右边界;
    if (currentRect.bottomRight().x() + movePoint.x() > m_screen_width)
    {
        movePoint.setX(m_screen_width - currentRect.bottomRight().x());
    }
    //移动选区是否超出屏幕下边界;
    if (currentRect.bottomRight().y() + movePoint.y() > m_screen_height)
    {
        movePoint.setY(m_screen_height - currentRect.bottomRight().y());
    }

    return movePoint;
}

StretchRectState CaptureScreen::getStrethRectState(QPoint point)
{
    StretchRectState stretchRectState = NotSelect;
    if (m_top_left_rect.contains(point))
    {
        stretchRectState = TopLeftRect;
    }
    else if (m_top_center_rect.contains(point))
    {
        stretchRectState = TopCenterRect;
    }
    else if (m_top_right_rect.contains(point))
    {
        stretchRectState = TopRightRect;
    }
    else if (m_right_center_rect.contains(point))
    {
        stretchRectState = RightCenterRect;
    }
    else if (m_bottom_right_rect.contains(point))
    {
        stretchRectState = BottomRightRect;
    }
    else if (m_bottom_center_rect.contains(point))
    {
        stretchRectState = BottomCenterRect;
    }
    else if (m_bottom_left_rect.contains(point))
    {
        stretchRectState = BottomLeftRect;
    }
    else if (m_left_center_rect.contains(point))
    {
        stretchRectState = LeftCenterRect;
    }

    return stretchRectState;
}

void CaptureScreen::setStretchCursorStyle(StretchRectState stretchRectState)
{
    switch (stretchRectState)
    {
    case NotSelect:
        setCursor(Qt::ArrowCursor);
        break;
    case TopLeftRect:
    case BottomRightRect:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case TopRightRect:
    case BottomLeftRect:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case LeftCenterRect:
    case RightCenterRect:
        setCursor(Qt::SizeHorCursor);
        break;
    case TopCenterRect:
    case BottomCenterRect:
        setCursor(Qt::SizeVerCursor);
        break;
    default:
        break;
    }
}

QRect CaptureScreen::getStretchRect()
{
    QRect stretchRect;
    QRect currentRect = getRect(m_begin_point, m_end_point);
    switch (m_stretchRectState)
    {
    case NotSelect:
        stretchRect = getRect(m_begin_point, m_end_point);
        break;
    case TopLeftRect:
        {
            stretchRect = getRect(currentRect.bottomRight(), m_end_move_point);
        }
        break;
    case TopRightRect:
        {
            QPoint beginPoint = QPoint(currentRect.topLeft().x(), m_end_move_point.y());
            QPoint endPoint = QPoint(m_end_move_point.x(), currentRect.bottomRight().y());
            stretchRect = getRect(beginPoint, endPoint);
        }
        break;
    case BottomLeftRect:
        {
            QPoint beginPoint = QPoint(m_end_move_point.x() , currentRect.topLeft().y());
            QPoint endPoint = QPoint(currentRect.bottomRight().x(), m_end_move_point.y());
            stretchRect = getRect(beginPoint, endPoint);
        }
        break;
    case BottomRightRect:
        {
            stretchRect = getRect(currentRect.topLeft(), m_end_move_point);
        }
        break;
    case LeftCenterRect:
        {
            QPoint beginPoint = QPoint(m_end_move_point.x(), currentRect.topLeft().y());
            stretchRect = getRect(beginPoint, currentRect.bottomRight());
        }
        break;
    case TopCenterRect:
    {
        QPoint beginPoint = QPoint(currentRect.topLeft().x(), m_end_move_point.y());
        stretchRect = getRect(beginPoint, currentRect.bottomRight());
    }
        break;
    case RightCenterRect:
    {
        QPoint endPoint = QPoint(m_end_move_point.x(), currentRect.bottomRight().y());
        stretchRect = getRect(currentRect.topLeft(), endPoint);
    }
        break;
    case BottomCenterRect:
    {
        QPoint endPoint = QPoint(currentRect.bottomRight().x(), m_end_move_point.y());
        stretchRect = getRect(currentRect.topLeft(), endPoint);
    }
        break;
    default:
    {
        stretchRect = getRect(m_begin_point , m_end_point );
    }
        break;
    }

    // 拖动结束更新 m_begin_point , m_end_point;
    if (m_current_capture_state == FinishMoveStretchRect)
    {
        m_begin_point = stretchRect.topLeft();
        m_end_point = stretchRect.bottomRight();
    }

    return stretchRect;
}
