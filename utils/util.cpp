#include "util.h"

float Util::Util::windowsDpiScale()
{
#if _WIN32
    const float DEFAULT_DPI = 96.0;
    HDC screen = GetDC(0);
    FLOAT dpiX = static_cast<FLOAT>( GetDeviceCaps( screen, LOGPIXELSX ) );
    ReleaseDC( 0, screen );
    return round(dpiX / DEFAULT_DPI); // Qt::AA_EnableHighDpiScaling
#elif __APPLE__
    return 2;
#endif
}
