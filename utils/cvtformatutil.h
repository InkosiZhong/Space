#ifndef CVTFORMATUTIL_H
#define CVTFORMATUTIL_H

#include <QPixmap>
#include "opencv2/opencv.hpp"

namespace Utils {

namespace CvtFormatUtil {

/*** Mat2QImage
 *  const cv::Mat& - input opencv Mat
 *  QImage - output QImage
*/
QImage Mat2QImage(const cv::Mat& mat);


/*** QImage2Mat
 *  const QImage& - input QImage
 *  cv::Mat - output opencv Mat
*/
cv::Mat QImage2Mat(const QImage& img);


/*** Mat2QPixmap
 *  const cv::Mat& - input opencv Mat
 *  QPixmap - output QPixmap
*/
QPixmap Mat2QPixmap(const cv::Mat& mat);


/*** QPixmap2Mat
 *  const QPixmap& - input QPixmap
 *  cv::Mat - output opencv Mat
*/
cv::Mat QPixmap2Mat(const QPixmap& map);

/*** Mat2QImage
 *  const cv::Mat& - input opencv Mat
 *  QImage& - output QImage
 *  bool - if convertion successful
*/
bool Mat2QImage(const cv::Mat& src, QImage& dest);


/*** QImage2Mat
 *  const QImage& - input QImage
 *  cv::Mat& - output opencv Mat
 *  bool - if convertion successful
*/
bool QImage2Mat(const QImage& src, cv::Mat& dest);

}}

#endif // CVTFORMATUTIL_H
