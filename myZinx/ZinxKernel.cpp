#include "ZinxKernel.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>

ZinxKernel ZinxKernel::g_kernel;

ZinxKernel::ZinxKernel()
{
	// 这里最好放到单独的 bool Init 函数里， 如果 create 失败会有返回信息, 这边先这样
	int fd = epoll_create(10);
	if (fd >= 0) {
		m_epollFd = fd;
	}
}

ZinxKernel::~ZinxKernel()
{
	if (m_epollFd != -1) {
		close(m_epollFd);
	}
}

void ZinxKernel::Run()
{
	while (1) {
		// 1 等输入	--> epoll
		struct epoll_event astEvents[100];
		int ready_count = epoll_wait(m_epollFd, astEvents, 100, -1);
		if (ready_count == 0) {
			if (errno == EINTR) {
				// 被信号打断
				continue;
			}
		}
		// 2 调用通道的读取函数 ReadFd 或 写出函数 -> 判断 epoll 事件掩码
		for (int i = 0; i < ready_count; ++i) {
			if (astEvents[i].events & EPOLLIN) {
				// 需要处理外部输入的数据	-> STDOUT_FILENO 的 EPOLLIN 事件也不会阻塞吧， 设置个ReadFd函数看看
				// 但是这里一直有监听 STDOUT_FILENO 的 EPOLLIN 事件， 每次也都会返回, 但是读到的 content 为空
				// 同时标准输出通道类的 DataProcess 实现也为空
				// 1 读出数据
				IChannel* pChannel = static_cast<IChannel*>(astEvents[i].data.ptr);
				std::string content = pChannel->ReadFd();
				// 2 处理数据
				pChannel->DataProcess(content);
			}
			if (astEvents[i].events & EPOLLOUT) {
				// 需要处理外部输出的数据
				IChannel* pChannel = static_cast<IChannel*>(astEvents[i].data.ptr);
				pChannel->FlushOut();
				// 输出完后删除 epoll 的 EPOLLOUT 监听事件
				ModChannelDelOut(pChannel);
			}
		}
	}
}

void ZinxKernel::ModChannelAddOut(IChannel* _pChannel)
{
	struct epoll_event stEvent;
	stEvent.events = EPOLLIN | EPOLLOUT;
	stEvent.data.ptr = _pChannel;
	epoll_ctl(m_epollFd, EPOLL_CTL_MOD, _pChannel->GetFd(), &stEvent);
}

void ZinxKernel::ModChannelDelOut(IChannel* _pChannel)
{
	struct epoll_event stEvent;
	stEvent.events = EPOLLIN;
	stEvent.data.ptr = _pChannel;
	epoll_ctl(m_epollFd, EPOLL_CTL_MOD, _pChannel->GetFd(), &stEvent);
}

void ZinxKernel::AddChannel(IChannel* _pChannel)
{
	struct epoll_event stEvent;
	stEvent.events = EPOLLIN;
	stEvent.data.ptr = _pChannel;
	epoll_ctl(m_epollFd, EPOLL_CTL_ADD, _pChannel->GetFd(), &stEvent);
}

void ZinxKernel::DelChannel(IChannel* _pChannel)
{
	epoll_ctl(m_epollFd, EPOLL_CTL_DEL, _pChannel->GetFd(), NULL);
}
