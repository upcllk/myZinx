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
	// ��ֱ�ӵ��� WriteFd�� ֻ�Ǵ浽���� m_buffer ���޸� epoll, �ȵ� epoll �����ٷ�, ͨ�ú���
	void DataSendOut(std::string _output);
	// ����ˢ���������� �ڲ����� WriteFd ����������������    ����
	void FlushOut();
	void WriteFd(std::string _output);

private:
	std::string m_buffer;
};

