#include "HalconMatcher.h"

#include "ScaledShapeMatch.h"

using namespace HDevEngineCpp;

HalconMatcher::HalconMatcher()
{
	std::string resourcePath = "C:/Users/admin/source/repos/DNNCV/HalconMatcher";
	ScaledShapeMatch::SetResourcePath(resourcePath.c_str());
	//  "execute_procedures_jit_compiled"  [default: false, 0]
	//     - if set to true (or "true"), procedures are tried to being compiled
	//       with a just-in-time compiler for faster execution
	//HDevEngine().SetEngineAttribute("execute_procedures_jit_compiled", true);

	//HDevEngine().StartDebugServer();
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
		HTuple  rows, cols, angles, scales, scores;
		auto f = [this, imgSrc, i, &rows, &cols, &angles, &scales, &scores](size_t modelID) {
			doMatching(imgSrc, models[i], rows, cols, angles, scales, scores);
			return std::make_pair(modelID, scores);
		};
		futureResults.emplace_back(pool.enqueue(f, i));//start a matching task for every single model.
	}

	byte bits = 0;
	double threshold = 0.8;
	int modelID = -1;
	double score = 0;
	for (auto&& result : futureResults)
	{
		try
		{
			std::pair<int, HTuple> p = result.get();//block & wait until all matching tasks finished.
			modelID = p.first;
			score = p.second[0].D();
			if (score > threshold) { bits |= 1 << modelID; }
		}
		catch (...)
		{
			std::cout << "Exception in getting matched result of modelID£º" << modelID << std::endl;
		}
	}

	return bits;
}

void HalconMatcher::doMatching(const HalconCpp::HObject& imgSrc, const HTuple& model, 
	HTuple& rows, HTuple& cols, HTuple& angles, HTuple& scales, HTuple& scores)
{
	long t1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	static std::mutex queue_mutex;
	std::unique_lock<std::mutex> lock(queue_mutex);
	ScaledShapeMatch::scaledShapeMatch(imgSrc, model, &rows, &cols, &angles, &scales, &scores);
	long t2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	std::cout << "\nThreadID::" << std::this_thread::get_id();
	std::cout << ",matching time elapsed:" << t2 - t1 << "ms" << std::endl;
}
