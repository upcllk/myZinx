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
	// 如果消息的方向是输入 -> 负责读数据, 如果是输出 -> 负责写数据
	SysIODirMsg* pMsg = dynamic_cast<SysIODirMsg*>(_inputMsg);
	if (pMsg != nullptr) {
		if (pMsg->m_Dir == SysIODirMsg::IO_IN) {
			// 读入数据交给业务层处理 ( ByteMsg 类型)
			std::string input_data = ReadFd();
			// ByteMsg(std::string _content, SysIODirMsg& _IO_Dir)
			if (!input_data.empty()) {
				pRet = new ByteMsg(input_data, *pMsg);
			}
		}
		else if (pMsg->m_Dir == SysIODirMsg::IO_OUT) {
			// _inputMsg 为业务层发过来的 ByteMsg 消息, 数据直接写出，没有下一环节

			//To do :  不对吧应该在转换成 ByteMsg 调用 SendOut() 吧
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
