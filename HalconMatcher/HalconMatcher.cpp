#include "HalconMatcher.h"

#include "ScaledShapeMatch.h"

using namespace HDevEngineCpp;

HalconMatcher::HalconMatcher()
{
	itemppath = "D:/temp/temp/wall2.jpg";
	imgpath = "D:/temp/temp/wall2.jpg";
	resourcePath = "C:/Users/jizhi/source/repos/DNNCV/HalconMatcher";
	ScaledShapeMatch::SetResourcePath(resourcePath.c_str());
}

HalconMatcher::~HalconMatcher()
{
}

int HalconMatcher::addModel(std::string imgTemplatePath)
{
	try
	{
		HImage imageTemplate(imgTemplatePath.c_str());
		HTuple modelID;
		ScaledShapeMatch::createTemplateImg(imageTemplate, &modelID);
		models.push_back(modelID);
	}
	catch (...)
	{

	}
	return models.size();
}

int HalconMatcher::clearAllModel()
{
	models.clear();
	return 0;
}

int HalconMatcher::doMatching(HObject& imgSrc)
{
	long t1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	std::vector< std::future<std::pair<int, HTuple>> > futureResults;
	int i = 0;
	for (auto& model : models) {
		HTuple  rows, cols, angles, scales, scores;
		auto f = [&](int modelTypeid) {
			doMatching(model, imgSrc, rows, cols, angles, scales, scores);
			return std::make_pair(modelTypeid, scores);
		};
		futureResults.emplace_back(pool.enqueue(f, i++));
	}
	byte bits = 0;
	double threshold = 0.8;
	for (auto&& result : futureResults)
	{
		try
		{
			std::pair<int, HTuple> p = result.get();//block & wait until all matching tasks finished.
			int modelTypeid = p.first;
			double score = p.second[0].D();
			if (score > threshold) { bits |= 1 << modelTypeid; }
		}
		catch (...)
		{
			//bits = 0;
		}
	}
	long t2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	std::cout << "\nTotal matching time elapsed:" << t2 - t1 << "ms" << std::endl;
	std::cout << "\nvisibility:" << (int)bits << std::endl;
	return bits;
}

void HalconMatcher::doMatching(HTuple& ModelID, HObject& image, HTuple& rows, HTuple& cols, HTuple& angles, HTuple& scales, HTuple& scores)
{
		long t1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		try
		{
		ScaledShapeMatch::scaled_shape_match(image, ModelID, &rows, &cols, &angles, &scales, &scores);

		long t2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		
		std::cout <<
			"\nthread_id:" << std::this_thread::get_id() << "," <<
			"\t(1)rows:" << rows[0].D() << "," <<
			"\t(2)cols:" << cols[0].D() << "," <<
			"\t(3)angles:" << angles[0].D() << "," <<
			"\t(4)scales:" << scales[0].D() << "," <<
			"\t(5)scores:" << scores[0].D() << "," <<
			"\t(6)time elapsed:" << t2 - t1 << "ms" << std::endl;

		}
		catch (...)
		{
				
		}
}