#pragma once

#include <vector>

#include <opencv2/imgproc/imgproc.hpp>

#include "MyTypedefs.hpp"

CONST SIZE_T MIN_CONTOUR_AREA = 100;

class Object final
{
private:
	std::vector<cv::Point> contour_;
	cv::Rect               rect_;
	FLOAT                  area_;

public:
	Object(CONST std::vector<cv::Point>&);

	auto getRect() const { return rect_; }

	BOOL isValid() const { return (area_ >= MIN_CONTOUR_AREA) ? TRUE : FALSE; }
};