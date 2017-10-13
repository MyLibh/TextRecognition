#pragma once

#include <iostream>
#include <sstream>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>

#include "MyTypedefs.hpp"

#include "Object.hpp"

cv::Mat FilterImage(CONST cv::Mat&, std::vector<std::vector<cv::Point>>&);

#pragma region Learning 

BOOL LearnSymbols();

BOOL SaveXml(CONST std::string&, CONST cv::Mat&);

#pragma endregion

#pragma region Recognition

BOOL FindSymbols(std::string* = new std::string()); // есть утечка?

BOOL ReadXml(CONST std::string&, cv::Mat&);

#pragma endregion