#include "IChannel.h"
#include "ZinxKernel.h"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>

IChannel::IChannel()
{
}

IChannel::~IChannel()
{
}

void IChannel::DataSendOut(std::string _output)
{
	m_buffer.append(_output.begin(), _output.end());
	ZinxKernel::GetInstance().ModChannelAddOut(this);
}

void IChannel::FlushOut()
{
	WriteFd(m_buffer);
	m_buffer.clear();
}

std::string IChannel::ReadFd()
{
	// 不设置非阻塞的话 STDOUT_FILENO 会阻塞在这里
	int flags = fcntl(this->GetFd(), F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(this->GetFd(), F_SETFL, flags);

	char buff[BUFSIZ];
	memset(buff, 0, sizeof(buff));
	int len = read(this->GetFd(), buff, sizeof(buff));
	if (len == -1) {
		// 标准输出的文件描述符 Resource temporarily unavailable
		// std::cout << strerror(errno) << std::endl;
		return std::string();
	}
	return std::string(buff, len);
}

void IChannel::WriteFd(std::string _output)
{
	// 这样写有隐患， 如果 _output 有 \0， c_str 后会提前结束
	// write(this->GetFd(), _output.c_str(), _output.size());
	char* pout = (char*)malloc(_output.size());
	_output.copy(pout, _output.size(), 0);
	write(this->GetFd(), pout, _output.size());
	free(pout);
}
