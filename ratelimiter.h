#ifndef _RATELIMITER_H_
#define _RATELIMITER_H_
#include <Poco/Mutex.h>
#include "iestype.h"
class CRateLimiter
{
public:
	CRateLimiter();
	~CRateLimiter();
	Juint32 acquire(Juint32 permits);//请求令牌，返回等待时间
	double getRate();
	void setRate(Juint32 rate); //单位 kb/s
private:
	void sync(Juint64 nowClock); //补充令牌
	Juint64 claim_next(double permits); //下次请求
private:
	double m_interval; //单字节所用微秒数
	double m_maxPermits;//最大令牌数量
	double m_storedPermits; //过去剩余的令牌
	Juint64 m_nextFree; //下次请求的承受时间点 小于承受时间点，计算出等待时间
	Poco::FastMutex m_mutex;
};
#endif