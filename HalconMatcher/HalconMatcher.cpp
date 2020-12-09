#include "HalconMatcher.h"

#include "ScaledShapeMatch.h"

using namespace HDevEngineCpp;

HMatcher::HMatcher()
{

}

HMatcher::~HMatcher()
{
	try
	{
	}
	catch (const std::exception&)
	{
			
	}

}

void HMatcher::doMatching(HTuple& ModelID, HObject& image, HTuple& rows, HTuple& cols, HTuple& angles, HTuple& scales, HTuple& scores)
{
	try
	{
		long t1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		ScaledShapeMatch::scaled_shape_match(image, ModelID, &rows, &cols, &angles, &scales, &scores);
		long t2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		std::cout <<
			"\trows:" << rows[0].D() << "," <<
			"\tcols:" << cols[0].D() << "," <<
			"\tangles:" << angles[0].D() << "," <<
			"\tscales:" << scales[0].D() << "," <<
			"\tscores:" << scores[0].D() << "," <<
			"\ttime elapsed:" << t2 - t1 << "ms" << std::endl;
	}
	catch (const std::exception&)
	{

	}

}
//void HMatcher::doMatching(HObject& imageTemplate, HObject& image, HTuple& rows, HTuple& cols, HTuple& angles, HTuple& scales, HTuple& scores)
//{
//	try
//	{
//		long t1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
//		HTuple* ModelID =	new HTuple();
//		ScaledShapeMatch::createTemplateImg(imageTemplate, ModelID);
//		ScaledShapeMatch::scaled_shape_match(image, *ModelID, &rows, &cols, &angles, &scales, &scores);
//		long t2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
//		std::cout << 
//			"\trows:" << rows[0].D() << "," <<
//			"\tcols:" << cols[0].D() << "," <<
//			"\tangles:" << angles[0].D() << "," <<
//			"\tscales:" << scales[0].D() << "," <<
//			"\tscores:" << scores[0].D() << "," <<
//			"\ttime elapsed:" << t2 - t1 << "ms" << std::endl;
//	}
//	catch (const std::exception&)
//	{
//
//	}
//
//}
