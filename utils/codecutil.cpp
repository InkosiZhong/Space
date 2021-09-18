#include "codecutil.h"

bool Util::CodecUtil::image2Base64(QImage& image, QString& base64){
    if (image.isNull())return false;
    QByteArray barray;
    QBuffer buffer(&barray);
    image.save(&buffer, "JPG");
    base64 = QString(barray.toBase64());
    return true;
}

bool Util::CodecUtil::pixmap2Base64(QPixmap& pixmap, QString& base64){
    QImage image = pixmap.toImage();
    return image2Base64(image, base64);
}
