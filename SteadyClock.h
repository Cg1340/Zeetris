#pragma once
#include <chrono>
#include <vector>
#include <algorithm>
using namespace std;
using namespace std::chrono;

class SteadyClock
{
public:
	void restart();//重新开始
	void pause();//计时器暂停
	void start();//计时器暂停
	long long getPauseTime();//获取暂停的时间（毫秒）
	long long getTime();//获取时间（毫秒）

private:
	steady_clock::time_point startPoi;
	steady_clock::time_point pausePoi;
	vector<long long> pauseTime;
};