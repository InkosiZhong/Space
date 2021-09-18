#ifndef CVTFORMATUTIL_H
#define CVTFORMATUTIL_H

#include <QPixmap>
#include "opencv2/opencv.hpp"

namespace Utils {

namespace CvtFormat {

QImage Mat2QImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage img(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        img.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            img.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = img.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return img;
    }
    else
    {
       return QImage();
    }
}

cv::Mat QImage2Mat(QImage& img)
{
    cv::Mat mat;
    switch(img.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(img.height(), img.width(), CV_8UC4, (void*)img.constBits(), img.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(img.height(), img.width(), CV_8UC3, (void*)img.constBits(), img.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(img.height(), img.width(), CV_8UC1, (void*)img.constBits(), img.bytesPerLine());
        break;
    default:
        break;
    }
    return mat;
}

QPixmap Mat2QPixmap(const cv::Mat& mat)
{
    return QPixmap::fromImage(Mat2QImage(mat));
}

cv::Mat QPixmap2Mat(QPixmap& map)
{
    QImage img = map.toImage();
    return QImage2Mat(img);
}


}

}

#endif // CVTFORMATUTIL_H
