#include "Object.hpp"

Object::Object(CONST std::vector<cv::Point> &crContour) :
	contour_(crContour),
	rect_(cv::boundingRect(crContour)),
	area_(static_cast<FLOAT>(cv::contourArea(crContour)))
{ }