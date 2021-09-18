#include "backgroundremover.h"

cv::Mat& BackgroundRemover::complementSrc(cv::Mat& src) {
	for (int i = 0; i < src.rows; ++i) {
		for (int j = 0; j < src.cols; ++j) {
			if (src.channels() == 3) {
                cv::Vec3b& src_color = src.at<cv::Vec3b>(i, j);
                src_color[0] = 255 - src_color[0]; // B
                src_color[1] = 255 - src_color[1]; // G
                src_color[2] = 255 - src_color[2]; // R
			}
			else if (src.channels() == 4) {
                cv::Vec4b& src_color = src.at<cv::Vec4b>(i, j);
                src_color[0] = 255 - src_color[0]; // B
                src_color[1] = 255 - src_color[1]; // G
                src_color[2] = 255 - src_color[2]; // R
			}
		}
	}
	return src;
}

cv::Mat BackgroundRemover::handle(cv::Mat src, bool complement) {
    cv::Mat mask, dest;
    if (complement)src = complementSrc(src);
	cv::cvtColor(src, mask, cv::COLOR_BGR2GRAY);
    cv::threshold(mask, mask, m_threshold, 255, cv::THRESH_BINARY);
    cv::erode(mask, mask, 3);
    src.copyTo(dest, 255 - mask);
	return dest;
}

void BackgroundRemover::setThreshold(const int& threshold){
    m_threshold = threshold;
}
