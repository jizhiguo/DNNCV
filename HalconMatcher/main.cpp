#include "HalconMatcher.h"

int main(int argc, char* argv[])
{

    std::string imgSrcPath = "D:/temp/temp/wall2.jpg";
    HImage imgSrc(imgSrcPath.c_str());

    int v;
    int m;
    HalconMatcher halconMatcher;
    m = halconMatcher.addModel("D:/temp/temp/wall2.jpg");
    m = halconMatcher.addModel("D:/temp/temp/wall2.jpg");
    m = halconMatcher.addModel("D:/temp/temp/p11.jpg");
    m = halconMatcher.addModel("D:/temp/temp/wall20.jpg");
    v = halconMatcher.doMatching(imgSrc);

    std::cout << "m:" << m << std::endl;
    std::cout << "v:" << v << std::endl;
    //VideoVisibility::getInstance()->SetVisibilityValue("", v);

    return 0;
}
