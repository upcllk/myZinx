#include "ZinxKernel.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>

ZinxKernel ZinxKernel::g_kernel;

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
		// 2 ����ͨ���Ķ�ȡ���� ReadFd �� д������ -> �ж� epoll �¼�����
		for (int i = 0; i < ready_count; ++i) {
			if (astEvents[i].events & EPOLLIN) {
				// ��Ҫ�����ⲿ���������	-> STDOUT_FILENO �� EPOLLIN �¼�Ҳ���������ɣ� ���ø�ReadFd��������
				// ��������һֱ�м��� STDOUT_FILENO �� EPOLLIN �¼��� ÿ��Ҳ���᷵��, ���Ƕ����� content Ϊ��
				// ͬʱ��׼���ͨ����� DataProcess ʵ��ҲΪ��
				// 1 ��������
				IChannel* pChannel = static_cast<IChannel*>(astEvents[i].data.ptr);
				std::string content = pChannel->ReadFd();
				// 2 ��������
				pChannel->DataProcess(content);
			}
			if (astEvents[i].events & EPOLLOUT) {
				// ��Ҫ�����ⲿ���������
				IChannel* pChannel = static_cast<IChannel*>(astEvents[i].data.ptr);
				pChannel->FlushOut();
				// ������ɾ�� epoll �� EPOLLOUT �����¼�
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
