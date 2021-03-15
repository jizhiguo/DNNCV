#include "HalconMatcher.h"

int main(int argc, char* argv[])
{
    int v;
    int m;
    HalconMatcher halconMatcher;
    m = halconMatcher.addModel("d:/temp/temp/wall2.jpg");
    m = halconMatcher.addModel("d:/temp/temp/wall1.jpg");
    m = halconMatcher.addModel("d:/temp/temp/wall2.jpg");
    std::string imgSrcPath = "d:/temp/temp/wall2.jpg";
    HImage imgSrc(imgSrcPath.c_str());
    long t1;
    long t2;
    t1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    v = halconMatcher.doMatching(imgSrc);
    t2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "\n*Total matching time elapsed:" << t2 - t1 << "ms" << std::endl;
    std::cout << "\n*visibility:" << v << std::endl;
    //VideoVisibility::getInstance()->SetVisibilityValue("videoID", v);

    return 0;
}
