#pragma once
#include <HalconCpp.h>
#include "ThreadPool.h"
#include <HDevEngineCpp.h>
using namespace HalconCpp;

class HMatcher
{
private:

public:
	
	HMatcher();
	~HMatcher();
	void doMatching(HObject& imageTemplate, HObject& image, HTuple& rows, HTuple& cols, HTuple& angles, HTuple& scales, HTuple& scores);

};

