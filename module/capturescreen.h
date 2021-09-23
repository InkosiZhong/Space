#include <QWidget>
#include <QPainter>
#include <QDebug>

#include "constants.h"
#include "utils/util.h"

extern enum CaptureState captureState;
extern enum StretchRectState stretchRectState;

struct DataPackage{
    QPixmap* src;
    QPoint* ref_point;
    int screen_id;
    short corner; // choose from CornerIndex
};

class CaptureScreen : public QWidget
{
    Q_OBJECT

public:
    CaptureScreen(QWidget *parent = 0);
    ~CaptureScreen();

    void active();

Q_SIGNALS:
    void signalFinishCapture(DataPackage* data);
    void signalCancelCapture(); // when click nothing

private:
    void initWindow();
    void initStretchRect();
    void loadBackgroundPixmap();
    QRect getRect(const QPoint &beginPoint, const QPoint &endPoint);
    QRect getMoveRect();
    QRect getStretchRect();
    bool isPressPointInSelectRect(QPoint mousePressPoint);
    QRect getSelectRect();
    QPoint getMovePoint();
    StretchRectState getStrethRectState(QPoint point);
    void setStretchCursorStyle(StretchRectState stretchRectState);

    void drawCaptureImage();
    void drawStretchRect();

    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void onCaptureCompleted();

private:
    //bool m_is_mouse_press;
    QPixmap m_load_pixmap, m_capture_pixmap;
    int m_screen_width;
    int m_screen_height;
    QPoint m_begin_point, m_end_point, m_begin_move_point, m_end_move_point;
    QPainter m_painter;
    CaptureState m_current_capture_state = InitCapture;
    QRect m_current_select_rect;
    QRect m_top_left_rect, m_top_right_rect, m_bottom_left_rect, m_bottom_right_rect;
    QRect m_left_center_rect, m_top_center_rect, m_right_center_rect, m_bottom_center_rect;
    StretchRectState m_stretchRectState;
    DataPackage m_data;
};
