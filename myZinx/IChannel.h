#pragma once
#include <string>

class IChannel
{
public:
	IChannel();
	virtual ~IChannel();
	virtual std::string ReadFd() = 0;

	virtual int GetFd() = 0;
	virtual void DataProcess(std::string _input) = 0;
	// 不直接调用 WriteFd， 只是存到缓存 m_buffer 并修改 epoll, 等到 epoll 返回再发, 通用函数
	void DataSendOut(std::string _output);
	// 向外刷出缓存数据 内部调用 WriteFd 这个才是真正的输出    函数
	void FlushOut();
	void WriteFd(std::string _output);

private:
	std::string m_buffer;
};

