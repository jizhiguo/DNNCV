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
	/// ׼��ģ�塣
	/// </summary>
	/// <param name="itemppath">ģ��ͼƬ·��</param>
	/// <returns>ģ�����</returns>
	int addModel(std::string imgTemplatePath);
	int clearAllModel();

	/// <summary>
	/// ����Դͼ��
	/// </summary>
	/// <param name="imgSrc">Դͼ</param>
	/// <returns>ģ���Ŷ�Ӧ������λ��
	/// ���磺
	/// bit��
	/// ---+---+---+---+---+---+---+---+
	///	 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
	/// ---+---+---+---+---+---+---+---+
	/// ---+---+---+---+---+---+---+---+
	///	 0 | 0 | 0 | 0 | 0 | 1 | 1 | 0 |
	/// ---+---+---+---+---+---+---+---+
	///                      ^   ^   ^
	///                      |   |   |
	///                      |   |   +--------0��ģ��û��ƥ��
	///                      |   +------------1��ģ����ƥ��
	///                      +----------------2��ģ����ƥ��
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

