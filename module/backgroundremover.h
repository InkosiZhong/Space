#ifndef BACKGROUND_REMOVER_H
#define BACKGROUND_REMOVER_H

#include "opencv2/opencv.hpp"

class BackgroundRemover {
public:
	// return the same mat of arg, just for user friendly
    cv::Mat handle(cv::Mat src, bool complement = false);
    void setThreshold(const int& threshold);

private:
    cv::Mat& complementSrc(cv::Mat& src);

private:
    int m_threshold = 127;
};

#endif
