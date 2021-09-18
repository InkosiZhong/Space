#ifndef CODECUTIL_H
#define CODECUTIL_H

#include <QString>
#include <QImage>
#include <QPixmap>
#include <QBuffer>
#include <QByteArray>

namespace Util {

namespace CodecUtil
{

bool image2Base64(QImage& image, QString& base64);

bool pixmap2Base64(QPixmap& pixmap, QString& base64);

}

}

#endif // CODECUTIL_H
