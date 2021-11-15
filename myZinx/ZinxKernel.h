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
	// 添加通道顺便将通道 fd 和 epoll 关联起来
	void AddChannel(StdinChannel* _pChannel);
	// 删除通道顺便将通道 fd 移除 epoll
	void DelChannel(StdinChannel* _pChannel);
	StdinChannel* GetChannel() {
		return m_channel;
	}
};


