#ifndef _RATELIMITER_H_
#define _RATELIMITER_H_
#include <Poco/Mutex.h>
#include "iestype.h"
class CRateLimiter
{
public:
	CRateLimiter();
	~CRateLimiter();
	Juint32 acquire(Juint32 permits);//�������ƣ����صȴ�ʱ��
	double getRate();
	void setRate(Juint32 rate); //��λ kb/s
private:
	void sync(Juint64 nowClock); //��������
	Juint64 claim_next(double permits); //�´�����
private:
	double m_interval; //���ֽ�����΢����
	double m_maxPermits;//�����������
	double m_storedPermits; //��ȥʣ�������
	Juint64 m_nextFree; //�´�����ĳ���ʱ��� С�ڳ���ʱ��㣬������ȴ�ʱ��
	Poco::FastMutex m_mutex;
};
#endif