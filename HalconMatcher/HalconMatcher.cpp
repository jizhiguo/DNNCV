#include "HalconMatcher.h"

#include "ScaledShapeMatch.h"

using namespace HDevEngineCpp;

HalconMatcher::HalconMatcher()
{
	std::string resourcePath = "C:/Users/admin/source/repos/DNNCV/HalconMatcher";
	ScaledShapeMatch::SetResourcePath(resourcePath.c_str());
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
		std::cout << "Exception in creating model£º" << imgTemplatePath << std::endl;
	}
	return models.size();
}

int HalconMatcher::doMatching(HObject const& imgSrc)
{
	std::vector< std::future<std::pair<size_t, HTuple>> > futureResults;
	size_t len = models.size();
	for (size_t i = 0; i < len; i++) {
		HTuple model, rows, cols, angles, scales, scores;
		model = models[i].Clone();
		auto f = [this, imgSrc, model, &rows, &cols, &angles, &scales, &scores](size_t modelID) {
			ScaledShapeMatch::scaled_shape_match(imgSrc, model, &rows, &cols, &angles, &scales, &scores);
			return std::make_pair(modelID, scores);
		};
		futureResults.emplace_back(pool.enqueue(f, i));//start a task for every single model.
	}

	byte bits = 0;
	double threshold = 0.8;
	int modelTypeid = 0;
	double score = 0;
	for (auto&& result : futureResults)
	{
		try
		{
			std::pair<int, HTuple> p = result.get();//block & wait until all matching tasks finished.
			modelTypeid = p.first;
			score = p.second[0].D();
			if (score > threshold) { bits |= 1 << modelTypeid; }
		}
		catch (...)
		{
		}
	}
	return bits;
}