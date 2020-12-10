#include "HalconMatcher.h"

int main(int argc, char* argv[])
{

    std::string imgSrcPath = "F:/temp/wall2.jpg";
    HImage imgSrc(imgSrcPath.c_str());

    int v;
    int m;
    HalconMatcher halconMatcher;
    m = halconMatcher.addModel("F:/temp/wall2.jpg");
    m = halconMatcher.addModel("F:/temp/wall1.jpg");
    m = halconMatcher.addModel("F:/temp/wall2.jpg");
    m = halconMatcher.addModel("F:/temp/wall1.jpg");
    m = halconMatcher.addModel("F:/temp/wall2.jpg");
    m = halconMatcher.addModel("F:/temp/wall2.jpg");
    m = halconMatcher.addModel("F:/temp/wall2.jpg");
    m = halconMatcher.addModel("F:/temp/wall2.jpg");
    m = halconMatcher.addModel("F:/temp/wall2.jpg");
    m = halconMatcher.addModel("F:/temp/wall2.jpg");
    int v1 = -1;
    long t1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    long t2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    for (size_t i = 0; i < 1000000; i++)
    {
        t1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        v = halconMatcher.doMatching(imgSrc);
        if (v1 > 0 && v1 != v) {
            throw;
        }
        v1 = v;
        t2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        std::cout << "\n*Total matching time elapsed:" << t2 - t1 << "ms" << std::endl;
        std::cout << "\n*visibility:" << v << std::endl;
    }
    //VideoVisibility::getInstance()->SetVisibilityValue("", v);

    return 0;
}
