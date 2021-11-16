#pragma once
#include "StdinChannel.h"
#include "IChannel.h"
#include <list>

// 全局单例
class ZinxKernel
{
private:
	std::list<IChannel*> m_channelList;
	int m_epollFd = -1;
	ZinxKernel();
	virtual ~ZinxKernel();
	static ZinxKernel g_kernel;
public:
	static ZinxKernel& GetInstance() {
		return g_kernel;
	}
	void Run();
	// 添加通道顺便将通道 fd 和 epoll 关联起来
	void AddChannel(IChannel* _pChannel);
	// 删除通道顺便将通道 fd 移除 epoll
	void DelChannel(IChannel* _pChannel);
	// 修改 epoll 的监听类型
	// 添加输出的监听方向
	void ModChannelAddOut(IChannel* _pChannel);
	// 删除输出的监听方向
	void ModChannelDelOut(IChannel* _pChannel);
};


