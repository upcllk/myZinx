#include "ZinxKernel.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>

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
		// 2 调用输入通道的读取函数 ReadFd
		for (int i = 0; i < ready_count; ++i) {
			StdinChannel* pChannel = static_cast<StdinChannel*>(astEvents[i].data.ptr);
			std::string content = pChannel->ReadFd();
			// 3 回显到标准输出
			std::cout << content << std::endl;
		}
	}
}

void ZinxKernel::AddChannel(StdinChannel* _pChannel)
{
	struct epoll_event stEvent;
	stEvent.events = EPOLLIN;
	stEvent.data.ptr = _pChannel;
	epoll_ctl(m_epollFd, EPOLL_CTL_ADD, STDIN_FILENO, &stEvent);
}

void ZinxKernel::DelChannel(StdinChannel* _pChannel)
{
	epoll_ctl(m_epollFd, EPOLL_CTL_DEL, STDIN_FILENO, NULL);
}
