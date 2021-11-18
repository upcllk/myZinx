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

void IChannel::SetNextStage(AZinxHandler* _nextHandler)
{
	SetNextHandler(_nextHandler);
}

AZinxHandler* IChannel::GetNextStage()
{
	return GetNextHandler();
}  

ZinxMessage* IChannel::InternelHandle(ZinxMessage* _inputMsg)
{
	ZinxMessage* pRet = nullptr;
	// �����Ϣ�ķ��������� -> ���������, �������� -> ����д����
	SysIODirMsg* pMsg = dynamic_cast<SysIODirMsg*>(_inputMsg);
	if (pMsg != nullptr) {
		if (pMsg->m_Dir == SysIODirMsg::IO_IN) {
			// �������ݽ���ҵ��㴦�� ( ByteMsg ����)
			std::string input_data = ReadFd();
			// ByteMsg(std::string _content, SysIODirMsg& _IO_Dir)
			if (!input_data.empty()) {
				pRet = new ByteMsg(input_data, *pMsg);
			}
		}
		else if (pMsg->m_Dir == SysIODirMsg::IO_OUT) {
			// _inputMsg Ϊҵ��㷢������ ByteMsg ��Ϣ, ����ֱ��д����û����һ����

			//To do :  ���԰�Ӧ����ת���� ByteMsg ���� SendOut() ��
			ByteMsg* byteMsg = dynamic_cast<ByteMsg*>(pMsg);
			if (byteMsg != nullptr) {
				DataSendOut(byteMsg->m_content);
			}
		}
	}
	return pRet;
}

std::string IChannel::ReadFd()
{
	// �����÷������Ļ� STDOUT_FILENO ������������
	int flags = fcntl(this->GetFd(), F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(this->GetFd(), F_SETFL, flags);

	char buff[BUFSIZ];
	memset(buff, 0, sizeof(buff));
	int len = read(this->GetFd(), buff, sizeof(buff));
	if (len == -1) {
		// ��׼������ļ������� Resource temporarily unavailable
		// std::cout << strerror(errno) << std::endl;
		return std::string();
	}
	return std::string(buff, len);
}

void IChannel::WriteFd(std::string _output)
{
	// ����д�������� ��� _output �� \0�� c_str �����ǰ����
	// write(this->GetFd(), _output.c_str(), _output.size());
	char* pout = (char*)malloc(_output.size());
	_output.copy(pout, _output.size(), 0);
	write(this->GetFd(), pout, _output.size());
	free(pout);
}
