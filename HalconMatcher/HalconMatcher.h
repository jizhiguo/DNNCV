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
	/// ����ģ�塣
	/// </summary>
	/// <param name="itemppath">ģ��ͼƬ·��</param>
	/// <returns>ģ�����</returns>
	int addModel(std::string imgTemplatePath);

	/// <summary>
	/// ʹ��ģ����ƥ��Դͼ�����ظ���ģ���ƥ������
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
	///             ..........................n��ģ����û��ƥ�䣿
	/// </returns>
	int doMatching(HObject const& imgSrc);

	HalconMatcher();

private:
	std::vector<HTuple> models;
	ThreadPool pool;
};

