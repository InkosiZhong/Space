#include "constants.h"

const short LEFT = 0b0001, RIGHT=0b0010, TOP=0b0100, BOTTOM=0b1000;
const int STRETCH_DIAMETER = 10;

const int RM_BEGIN = 3;
const int RM_BACK = -3;

const short PROMPT_MATHPIX = 0b01;

const QString EMPTY_STRING = "*NONE*";

const QColor color_shadow = QColor(0, 0, 0, 100);
const QColor color_blue_gray = QColor(51, 63, 80);
const QColor color_bright_gray = QColor(233, 237, 244);

const QColor color_accent = color_blue_gray;

QString level_prefix[3] = {
    "Hint: ",
    "Warning: ",
    "Error: "
};

QString level_stylesheet[3] = {
    "background-color:rgb(233, 237, 244); color:rgb(50, 50, 50)",
    "background-color:rgb(255, 200, 0); color:rgb(50, 50, 50)",
    "background-color:rgb(255, 0, 80); color:rgb(255, 255, 255)"
};

QString ocr_area_stylesheet[2] = {
    "QCheckBox::indicator{ \
         width: 0px \
     } \
     QCheckBox{ \
         background-color: rgba(105, 195, 146, 0); \
         border: 1px solid rgb(105, 195, 146); \
     }",
    "QCheckBox::indicator{ \
         width: 0px \
     } \
     QCheckBox{ \
         background-color: rgba(105, 195, 146, 0.4); \
         border: 1px solid rgb(105, 195, 146); \
     }"
};

enum CaptureState;
enum StretchRectState;

enum LatexType;

#if _WIN32
enum HBitmapFormat;

enum ClipboardFormat;
#endif

enum Operations;
Q_DECLARE_METATYPE(Operations);

enum PromptLevel;

enum SmartSpaceMode;
Q_DECLARE_METATYPE(SmartSpaceMode);
