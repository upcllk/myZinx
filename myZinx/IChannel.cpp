#include "IChannel.h"
#include "ZinxKernel.h"
#include <unistd.h>

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

void IChannel::WriteFd(std::string _output)
{
	_output.push_back('\n');
	write(this->GetFd(), _output.c_str(), _output.size());
}
