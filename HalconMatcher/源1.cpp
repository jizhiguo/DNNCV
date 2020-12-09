#include "HalconCpp.h"
#include "HDevEngineCpp.h"
#include "HalconMatcher.h"
#include "ScaledShapeMatch.h"
#include <map>

using namespace HalconCpp;
using namespace HDevEngineCpp;


int main(int argc, char* argv[])

{
    ThreadPool pool;
    std::vector< std::future<std::pair<int, HTuple>> > results;
    
    HMatcher halconMatcher;
    HImage itemp("F:/temp/wall2.jpg");
    HImage  img("F:/temp/wall1.jpg");
    HTuple rows;
    HTuple cols;
    HTuple angles;
    HTuple scales;
    HTuple scores;
    ScaledShapeMatch::SetResourcePath("C:/Users/Admin/source/repos/DNNCV/HalconMatcher");
    HTuple* ModelID = new HTuple();
    ScaledShapeMatch::createTemplateImg(itemp, ModelID);
    auto f = [&](int modelTypeid) {
        halconMatcher.doMatching(*ModelID, img, rows, cols, angles, scales, scores);
        return std::make_pair(modelTypeid, scores); 
    };

    long t1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    for (size_t i = 0; i < 3; i++)
    {
        //results.emplace_back(pool.enqueue(ScaledShapeMatch::scaled_shape_match, img, *ModelID, &rows, &cols, &angles, &scales, &scores));
        results.emplace_back(pool.enqueue(f,i));
    }
    byte    bits = 0;
    for (auto&& result : results)
    {
        double threshold = 0.8;
        std::pair<int, HTuple> p = result.get();
        int modelTypeid = p.first;
        double score= p.second[0].D();
        if (score > threshold) { bits |= 1 << modelTypeid; }
    }
    long t2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout <<
        "\n\t(total)time elapsed:" << t2 - t1 << "ms" << std::endl;


    return 0;
}
