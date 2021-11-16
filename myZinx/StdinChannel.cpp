#include "StdinChannel.h"
#include "StdoutChannel.h"
#include <iostream>
#include <unistd.h>

StdinChannel::StdinChannel()
{
}

StdinChannel::~StdinChannel()
{
}

std::string StdinChannel::ReadFd()
{
	std::string ret;
	getline(std::cin, ret);
	return ret;
}

int StdinChannel::GetFd()
{
	return STDIN_FILENO;
}

void StdinChannel::DataProcess(std::string _input)
{
	// �����ݻ��Ե���׼��� -> ��Ҫ���ñ�׼���ͨ������� SendOut ����
	m_outChannel->DataSendOut(_input);
}