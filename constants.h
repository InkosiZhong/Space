#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QColor>
#include <QMetaType>

extern const short LEFT, RIGHT, TOP, BOTTOM;
extern const int STRETCH_DIAMETER;

extern const int RM_BEGIN;
extern const int RM_BACK;

extern const QString EMPTY_STRING;

extern const short PROMPT_MATHPIX;

extern QString level_prefix[3];

extern QString level_stylesheet[3];

extern QString ocr_area_stylesheet[3][2];

extern const QColor color_shadow;
extern const QColor color_blue_gray;
extern const QColor color_bright_gray;

extern const QColor color_accent;

enum CaptureState{
    InitCapture,
    BeginCaptureImage,
    FinishCaptureImage,
    BeginMoveCaptureArea,
    FinishMoveCaptureArea,
    BeginMoveStretchRect,
    FinishMoveStretchRect
};

enum StretchRectState{
    NotSelect,
    TopLeftRect,
    TopRightRect,
    BottomLeftRect,
    BottomRightRect,
    LeftCenterRect,
    TopCenterRect,
    RightCenterRect,
    BottomCenterRect
};

enum LatexType {
    LATEX_PRINTED,
    LATEX_HANDWRITED,
    LATEX_UNKOWN };

#if WIN32
enum HBitmapFormat { // should in QWin::
    HBitmapNoAlpha,
    HBitmapPremultipliedAlpha,
    HBitmapAlpha
};
#endif
enum ClipboardFormat {
    WIN32_CF_BID,
    WIN32_PNG,
    WIN32_FILE
};

enum Operations {
    GetOperated,
    GetSrc,
    GetFormula,
    GetOCR,
    RollBack,
    ClearAll,
    Clean
};

enum PromptLevel {
    Lv_HINT,
    Lv_WARNING,
    Lv_ERROR,
    Lv_NONE
};

enum SmartSpaceMode {
    SPACE_DISBALE,
    SPACE_AI,
    SPACE_IMITATE
};

extern const float MAIN_CONF;
extern const float LATEX_SIZE_PROP_THREAD;
extern const float OCR_DENSITY_THREAD;

enum Performance {
    PERFORM_LIGHTWEIGHT,
    PERFORM_BALANCE,
    PERFORM_SPEED,
    PERFORM_ADAPTIVE
};

enum CoarseGrained{
    CG_WORD,
    CG_LINE,
    CG_PARA
};

#endif // CONSTANTS_H
