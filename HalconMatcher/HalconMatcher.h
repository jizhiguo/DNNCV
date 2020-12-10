#pragma once
#include <HalconCpp.h>
#include "ThreadPool.h"
#include <HDevEngineCpp.h>
#include <list>
using namespace HalconCpp;

class HalconMatcher
{
public:
	/// <summary>
	/// 准备模板。
	/// </summary>
	/// <param name="itemppath">模板图片路径</param>
	/// <returns>模板个数</returns>
	int addModel(std::string imgTemplatePath);
	int clearAllModel();

	/// <summary>
	/// 搜索源图。
	/// </summary>
	/// <param name="imgSrc">源图</param>
	/// <returns>模板编号对应比特置位。
	/// 例如：
	/// bit：
	/// ---+---+---+---+---+---+---+---+
	///	 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
	/// ---+---+---+---+---+---+---+---+
	/// ---+---+---+---+---+---+---+---+
	///	 0 | 0 | 0 | 0 | 0 | 1 | 1 | 0 |
	/// ---+---+---+---+---+---+---+---+
	///                      ^   ^   ^
	///                      |   |   |
	///                      |   |   +--------0号模板没有匹配
	///                      |   +------------1号模板有匹配
	///                      +----------------2号模板有匹配
	/// </returns>
	int doMatching(HObject& imgSrc);

	HalconMatcher();
	~HalconMatcher();

private:
	void doMatching(HTuple& ModelID, HObject& image, HTuple& rows, HTuple& cols, HTuple& angles, HTuple& scales, HTuple& scores);

private:
	std::string itemppath;
	std::string imgpath;
	std::string resourcePath;
	std::list<HTuple> models;
	ThreadPool pool;
};

