#include "HalconCpp.h"
#include "HDevEngineCpp.h"
#include "HalconMatcher.h"
#include "ScaledShapeMatch.h"

using namespace HalconCpp;
using namespace HDevEngineCpp;


int main(int argc, char* argv[])

{
    ThreadPool pool;
    std::vector< std::future<void> > results;

    HMatcher halconMatcher;
    HImage itemp("D:/temp/temp/wall2.jpg");
    HImage  img("D:/temp/temp/wall2.jpg");
    HTuple rows;
    HTuple cols;
    HTuple angles;
    HTuple scales;
    HTuple scores;
    ScaledShapeMatch::SetResourcePath("C:/Users/jizhi/source/repos/DNNCV/HalconMatcher");
    HTuple* ModelID = new HTuple();
    ScaledShapeMatch::createTemplateImg(itemp, ModelID);
    auto f = [&] {halconMatcher.doMatching(*ModelID, img, rows, cols, angles, scales, scores); };
    for (size_t i = 0; i < 3; i++)
    {
        //results.emplace_back(pool.enqueue(ScaledShapeMatch::scaled_shape_match, img, *ModelID, &rows, &cols, &angles, &scales, &scores));
        results.emplace_back(pool.enqueue(f));
    }
    for (auto&& result : results)
        result.get();

    return 0;
}
