// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "TextRecognition.hpp"

#ifdef NDEBUG_
	#pragma comment(lib, "opencv_world340d.lib")
#else
	#pragma comment(lib, "opencv_world340.lib")
#endif // NDEBUG_

inline VOID ShowWindow(CONST std::string &crsTitle, CONST cv::Mat &crmImage)
{
#ifdef SHOW_ALL
	cv::imshow(crsTitle, crmImage);
	cv::waitKeyEx(0);
#endif // SHOW_ALL
}

cv::Mat FilterImageAndFindContours(CONST cv::Mat &crmImage, std::vector<std::vector<cv::Point>> &rContours)
{
	cv::Mat mImage = crmImage.clone();
	ShowWindow("Normal", mImage);

	cv::cvtColor(mImage, mImage, CV_BGR2GRAY);
	ShowWindow("Converted", mImage);

	cv::GaussianBlur(mImage, mImage, cv::Size(5, 5), NULL);
	ShowWindow("Gaussed", mImage);

	cv::adaptiveThreshold(mImage, mImage, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 11, 2);
	ShowWindow("Thresholded", mImage);

	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(mImage, rContours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	cv::destroyAllWindows();
	return mImage;
}

#pragma region Learning

BOOL LearnSymbols()
{
	INT tmp[] = 
	{
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
		'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
		'U', 'V', 'W', 'X', 'Y', 'Z' 
	};
	std::vector<INT> chars(tmp, tmp + sizeof(tmp) / sizeof(typeid(tmp)));

	cv::Mat mLearn = cv::imread("Resourses/LearnSymbols.png");         
	if(mLearn.empty())
	{                            
		std::cerr << "Cannot load image for learning\n"; 

		return FALSE;                                                 
	}

	std::vector<std::vector<cv::Point>> contours;
	FilterImageAndFindContours(mLearn, contours);

	cv::Mat mTrained,
		    mClassification;
	std::cout << "Begin to train\n";
	for(SIZE_T i = 0; i < contours.size(); ++i)
		if(cv::contourArea(contours[i]) > MIN_CONTOUR_AREA)
		{               
			cv::rectangle(mLearn, cv::boundingRect(contours[i]), cv::Scalar(255, 0, 0), 2); 

			cv::Mat mRect = mLearn(cv::boundingRect(contours[i]));
			cv::resize(mRect, mRect, cv::Size(20, 30));

			cv::imshow("Learning", mLearn);
			cv::imshow("Symbol", mRect);                              
			      
			auto key = cv::waitKeyEx(0);           
			std::cout << "\t" << key << std::endl;

			cv::rectangle(mLearn, cv::boundingRect(contours[i]), cv::Scalar(255, 255, 255), (MIN_CONTOUR_AREA >> 2) + 8);

			if(std::find(chars.begin(), chars.end(), key) != chars.end())
			{    
				mClassification.push_back(key);      
                      
				cv::Mat mFloat;
				mRect.convertTo(mFloat, CV_32FC1);

				cv::Mat mTmp = mFloat.reshape(1, 1);
				mTrained.push_back(mTmp);
			}   
		}    
	cv::destroyAllWindows();
	std::cout << "Training ended\nSaving\t";

	if(!SaveXml("Classifications", mClassification) || !SaveXml("Images", mTrained)) return FALSE;

	std::cout << "Success\n\n";
	return TRUE;
}

BOOL SaveXml(CONST std::string &crFilename, CONST cv::Mat &rMat)
{
	cv::FileStorage xml("Data/" + crFilename + ".xml", cv::FileStorage::WRITE);

	if(!xml.isOpened())
	{
		std::cerr << "Unable to open '" << crFilename << ".xml'\n";

		return FALSE;
	}

	xml << crFilename << rMat;
	xml.release();

	return TRUE;
}

#pragma endregion

#pragma region Recognition

BOOL FindSymbols(std::string *pResult /* = new std::string*/) 
{	       															
	cv::Mat mClassification, 
		    mTrained;
	if(!ReadXml("Classifications", mClassification) || !ReadXml("Images", mTrained)) return FALSE;

	cv::Ptr<cv::ml::KNearest> model(cv::ml::KNearest::create()); 
	model->train(mTrained, cv::ml::ROW_SAMPLE, mClassification);

	cv::Mat mFind = cv::imread("Resourses/Test3.png");
	if(mFind.empty())
	{
		std::cerr << "Cannot load image for finding\n";

		return FALSE;
	}

	std::vector<std::vector<cv::Point>> contours;
	cv::Mat tmp = FilterImageAndFindContours(mFind, contours);

	std::vector<Object> validContours;
	for(SIZE_T i = 0; i < contours.size(); ++i) 
	{              
		Object obj(contours[i]);
          
		if(obj.isValid()) validContours.push_back(obj);
	}
	       
	for(SIZE_T i = 0; i < validContours.size(); ++i)
	{           																
		cv::rectangle(mFind, validContours[i].getRect(), cv::Scalar(255, 0, 0), 2);

		cv::Mat mRect(tmp(validContours[i].getRect()));
		cv::resize(mRect, mRect, cv::Size(20, 30));

		cv::Mat mFloat;
		mRect.convertTo(mFloat, CV_32FC1);

		cv::Mat mChar(0, 0, CV_32F),
			    mTmp(mFloat.reshape(1, 1));
		model->findNearest(mTmp, 1, mChar);

		*pResult += static_cast<CHAR>(mChar.at<FLOAT>(0, 0));
	}
	cv::imshow("Result", mFind);
	
	std::cout << "Found symbols: " << *pResult << std::endl;

	cv::waitKeyEx(0);
	cv::destroyAllWindows();

	return TRUE;
}

BOOL ReadXml(CONST std::string &crFilename, cv::Mat &rMat)
{
	cv::FileStorage xml("Data/" + crFilename + ".xml", cv::FileStorage::READ);

	if (!xml.isOpened())
	{
		std::cerr << "Unable to open '" << crFilename << ".xml'\n";

		return FALSE;
	}

	xml[crFilename] >> rMat;
	xml.release();

	return TRUE;
}

#pragma endregion