#include <Poco/Clock.h>
#include <Poco/Thread.h>
#include <algorithm>
#include "ratelimiter.h"
CRateLimiter::CRateLimiter() : m_interval(0.0),m_maxPermits(0.0), m_storedPermits(0.0),m_nextFree(0)
{

}

CRateLimiter::~CRateLimiter()
{

}

Juint32 CRateLimiter::acquire(Juint32 permits)
{
	Juint64 wait_time = claim_next(permits);
	try
	{
		Poco::Thread::sleep(wait_time / 1000);
	}
	catch (Poco::Exception* e)
	{
		
	}
	catch (std::exception* e)
	{
	}
	catch (...)
	{
	}
	return wait_time / 1000;
}

double CRateLimiter::getRate()
{
	Poco::FastMutex::ScopedLock lock(m_mutex);
	if (abs(m_interval) < 0.0000001)
		return 0.0;
	return 1000000.0 / (double)m_interval;
}

void CRateLimiter::setRate(Juint32 rate)
{
	if (rate == 0)
		return;
	Poco::FastMutex::ScopedLock lock(m_mutex);
	m_maxPermits = (double)rate;
	m_interval = 1000000.0 / (double)rate;
}

void CRateLimiter::sync(Juint64 nowClock)
{
	if (nowClock > m_nextFree)
	{
		m_storedPermits = std::min(m_maxPermits, m_storedPermits + (nowClock - m_nextFree) / m_interval);
		m_nextFree = nowClock;
	}
}

Juint64 CRateLimiter::claim_next(double permits)
{
	Poco::FastMutex::ScopedLock lock(m_mutex);
	Poco::Clock nowClock;
	sync(nowClock.microseconds()); //补充令牌

	//等待时间大于等于0
	Juint64 wait = m_nextFree - nowClock.microseconds();

	//计算出下次有多少令牌
	double stored = std::min(permits, m_storedPermits);
	double fresh = permits - stored;

	//记录下一次请求允许时间间隔
	Juint64 next_free = (Juint64)(fresh * m_interval);

	m_nextFree += next_free;
	m_storedPermits -= stored;

	return wait;
}

