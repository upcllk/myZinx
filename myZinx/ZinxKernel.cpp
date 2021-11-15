#include "ZinxKernel.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>

ZinxKernel::ZinxKernel()
{
	// ������÷ŵ������� bool Init ����� ��� create ʧ�ܻ��з�����Ϣ, ���������
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
		// 1 ������	--> epoll
		struct epoll_event astEvents[100];
		int ready_count = epoll_wait(m_epollFd, astEvents, 100, -1);
		if (ready_count == 0) {
			if (errno == EINTR) {
				// ���źŴ��
				continue;
			}
		}
		// 2 ��������ͨ���Ķ�ȡ���� ReadFd
		for (int i = 0; i < ready_count; ++i) {
			StdinChannel* pChannel = static_cast<StdinChannel*>(astEvents[i].data.ptr);
			std::string content = pChannel->ReadFd();
			// 3 ���Ե���׼���
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
