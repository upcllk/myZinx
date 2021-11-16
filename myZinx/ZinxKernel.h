#pragma once
#include "StdinChannel.h"
#include "IChannel.h"
#include <list>

// ȫ�ֵ���
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
	// ���ͨ��˳�㽫ͨ�� fd �� epoll ��������
	void AddChannel(IChannel* _pChannel);
	// ɾ��ͨ��˳�㽫ͨ�� fd �Ƴ� epoll
	void DelChannel(IChannel* _pChannel);
	// �޸� epoll �ļ�������
	// �������ļ�������
	void ModChannelAddOut(IChannel* _pChannel);
	// ɾ������ļ�������
	void ModChannelDelOut(IChannel* _pChannel);
};


