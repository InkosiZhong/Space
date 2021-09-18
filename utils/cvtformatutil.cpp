# include "cvtformatutil.h"

QImage Utils::CvtFormatUtil::Mat2QImage(const cv::Mat& mat)
{
    QImage img;
    Mat2QImage(mat, img);
    return img;
}

cv::Mat Utils::CvtFormatUtil::QImage2Mat(const QImage& img)
{
    cv::Mat mat;
    QImage2Mat(img, mat);
    return mat;
}

QPixmap Utils::CvtFormatUtil::Mat2QPixmap(const cv::Mat& mat)
{
    return QPixmap::fromImage(Mat2QImage(mat));
}

cv::Mat Utils::CvtFormatUtil::QPixmap2Mat(const QPixmap& map)
{
    QImage img = map.toImage();
    return QImage2Mat(img);
}

bool Utils::CvtFormatUtil::Mat2QImage(const cv::Mat& src, QImage& dest)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(src.type() == CV_8UC1)
    {
        dest = QImage(src.cols, src.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        dest.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            dest.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = src.data;
        for(int row = 0; row < src.rows; row ++)
        {
            uchar *pDest = dest.scanLine(row);
            memcpy(pDest, pSrc, src.cols);
            pSrc += src.step;
        }
        return true;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(src.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)src.data;
        // Create QImage with same dimensions as input Mat
        dest = QImage(pSrc, src.cols, src.rows, src.step, QImage::Format_RGB888);
        dest.rgbSwapped();
        return true;
    }
    else if(src.type() == CV_8UC4)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)src.data;
        // Create QImage with same dimensions as input Mat
        dest = QImage(pSrc, src.cols, src.rows, src.step, QImage::Format_ARGB32);
        return true;
    }
    else
    {
       return false;
    }
}


bool Utils::CvtFormatUtil::QImage2Mat(const QImage& src, cv::Mat& dest)
{
    switch(src.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        dest = cv::Mat(src.height(), src.width(), CV_8UC4, (void*)src.constBits(), src.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        dest = cv::Mat(src.height(), src.width(), CV_8UC3, (void*)src.constBits(), src.bytesPerLine());
        cv::cvtColor(dest, dest, cv::COLOR_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        dest = cv::Mat(src.height(), src.width(), CV_8UC1, (void*)src.constBits(), src.bytesPerLine());
        break;
    default:
        return false;
    }
    return true;
}
