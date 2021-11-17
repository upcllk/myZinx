#pragma once
#include <string>
#include"AZinxHandler.h"


// epoll �� IChannel ֮�䴫�ݵ� IO �������Ϣ
class SysIODirMsg :
	public ZinxMessage
{
public:
	enum IO_DIR {
		IO_IN, IO_OUT
	} m_Dir = IO_IN;

	SysIODirMsg(IO_DIR dir) :m_Dir(dir) {}
};

// IChannel �� ҵ��㴫�ݵ� ByteMsg ��������
// Ϊ�������ݱ���֮ǰ��״̬����������һ�׶ε� IO ������Ϣ���������ĸ�ֵ���������Ǽ̳� SysIODirMsg ��
class ByteMsg :
	public SysIODirMsg
{
public:
	std::string m_content;

	ByteMsg(std::string _content, SysIODirMsg& _IO_Dir) :m_content(_content), SysIODirMsg(_IO_Dir.m_Dir) {}
};

// ����ͨ����� Handle ��ͳһ�Ĵ��������ͳ�ȥ
class IChannel :
	public AZinxHandler
{
public:
	IChannel();
	virtual ~IChannel();

	// �� kernel �� AddChannel() �е���
	virtual bool Init() = 0;
	// ȥ��ʼ���� �� kernel �� DelChannel() �е���
	virtual void Fini() = 0;
	virtual int GetFd() = 0;
	// ���Ŀǰ����ȷʵûɶ��,���������������?
	// ��һ�Ժ��Զ������������ InternelHandle ���ȵ��� DataProcess��? ������������߿���ָ��
	virtual void DataProcess(std::string _input) = 0;

	std::string ReadFd();
	// ��ֱ�ӵ��� WriteFd�� ֻ�Ǵ浽���� m_buffer ���޸� epoll, �ȵ� epoll �����ٷ�, ͨ�ú���
	void DataSendOut(std::string _output);
	// ����ˢ���������� �ڲ����� WriteFd ��������������������
	void FlushOut();

private:
	std::string m_buffer;
	// ͨ�� AZinxHandler �̳�
	virtual ZinxMessage* InternelHandle(ZinxMessage* _inputMsg) override;

	void WriteFd(std::string _output);
};

