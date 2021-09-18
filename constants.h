#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QColor>

extern const short LEFT, RIGHT, TOP, BOTTOM;
extern const int STRETCH_DIAMETER;

extern const int RM_BEGIN;
extern const int RM_BACK;

extern const QString EMPTY_STRING;

extern const short PROMPT_MATHPIX;

extern QString level_prefix[3];

extern QString level_stylesheet[3];

extern QString ocr_area_stylesheet[2];

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

enum Operations{
    GetSrc,
    GetOperated,
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


#endif // CONSTANTS_H
