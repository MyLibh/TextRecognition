// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Object.hpp"

Object::Object(CONST std::vector<cv::Point> &crContour) :
	contour_(crContour),
	rect_(cv::boundingRect(crContour)),
	area_(static_cast<FLOAT>(cv::contourArea(crContour)))
{ }