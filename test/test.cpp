// test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "../HalconMatcher/ThreadPool.h"

int main()
{
	ThreadPool pool;
	static 	std::vector<long> ticks;
	std::vector<std::future<void> > futureResults;
	size_t len = 10;
	for (size_t i = 0; i < len; i++) {
		
		auto f = [](size_t modelID) {
			ticks.emplace_back(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
			return ;
		};
		futureResults.emplace_back(pool.enqueue(f, i));//start a matching task for every single model.
	}

	for (auto&& result : futureResults)
	{
		try
		{
			result.get();//block & wait until all matching tasks finished.
		}
		catch (...)
		{
			std::cout << "Exception in getting result of futureResults" << result._Is_ready_at_thread_exit() << std::endl;
		}
	}
	size_t len1 = ticks.size();
	for (size_t i = 0; i < len1 - 1; i++)
	{
		std::cout << "\nNo.:" <<i<<","<< ticks[i + 1] - ticks[i] << "μs" << std::endl;
	}

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
