#pragma once
#include "StdinChannel.h"

class ZinxKernel
{
private:
	StdinChannel* m_channel = nullptr;
	int m_epollFd = -1;
public:
	ZinxKernel();
	virtual ~ZinxKernel();
	void Run();
	// ���ͨ��˳�㽫ͨ�� fd �� epoll ��������
	void AddChannel(StdinChannel* _pChannel);
	// ɾ��ͨ��˳�㽫ͨ�� fd �Ƴ� epoll
	void DelChannel(StdinChannel* _pChannel);
	StdinChannel* GetChannel() {
		return m_channel;
	}
};


