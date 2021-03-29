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
	/// 增加模板。
	/// </summary>
	/// <param name="itemppath">模板图片路径</param>
	/// <returns>模板个数</returns>
	int addModel(std::string imgTemplatePath);

	/// <summary>
	/// doMatching的单线程版本，参数含义见doMatching
	/// </summary>
	/// <param name="imgSrc"></param>
	/// <returns></returns>
	int doMatchingSingleThread(HObject const& imgSrc);

	/// <summary>
	/// 使用模板来匹配源图，返回各个模板的匹配结果。
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
	///             ..........................n号模板有没有匹配？
	/// </returns>
	int doMatching(HObject const& imgSrc);

	/// <summary>
	/// 设置Halcon脚本文件路径。
	/// </summary>
	/// <param name="halconResourcePath">Halcon脚本路径</param>
	/// <returns>原路径</returns>
	std::string setHalResPath(std::string halconResourcePath);

	HalconMatcher();

private:
	void doMatching(const HObject& imgSrc, const HTuple& model,
		HTuple& rows, HTuple& cols, HTuple& angles, HTuple& scales, HTuple& scores);

private:
	std::vector<HTuple> models;
	ThreadPool pool;
};

