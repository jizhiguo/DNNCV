#include "HalconCpp.h"
#include "HDevEngineCpp.h"
#include "HalconMatcher.h"

using namespace HalconCpp;
using namespace HDevEngineCpp;


int main(int argc, char* argv[])
{
    HMatcher halconMatcher;
    HImage itemp("F:/temp/wall2.jpg");
    HImage  img("F:/temp/wall2.jpg");
    HTuple rows;
    HTuple cols;
    HTuple angles;
    HTuple scales;
    HTuple scores;

    halconMatcher.doMatching(itemp, img, rows, cols, angles, scales, scores);
    halconMatcher.doMatching(itemp, img, rows, cols, angles, scales, scores);
    halconMatcher.doMatching(itemp, img, rows, cols, angles, scales, scores);
    halconMatcher.doMatching(itemp, img, rows, cols, angles, scales, scores);
    halconMatcher.doMatching(itemp, img, rows, cols, angles, scales, scores);


    return 0;
}
