#pragma once
#include <string>

class IChannel
{
public:
	IChannel();
	virtual ~IChannel();

	virtual int GetFd() = 0;
	virtual void DataProcess(std::string _input) = 0;

	std::string ReadFd();
	void WriteFd(std::string _output);
	// ��ֱ�ӵ��� WriteFd�� ֻ�Ǵ浽���� m_buffer ���޸� epoll, �ȵ� epoll �����ٷ�, ͨ�ú���
	void DataSendOut(std::string _output);
	// ����ˢ���������� �ڲ����� WriteFd ����������������    ����
	void FlushOut();

private:
	std::string m_buffer;
};

