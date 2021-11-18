#pragma once
#include "IChannel.h"


class TCPDataChannel :
    public IChannel
{
private:
    int m_cfd = -1;
public:
    TCPDataChannel(int _cfd);
    virtual ~TCPDataChannel();

    // ͨ�� IChannel �̳�
    virtual bool Init() override;
    virtual void Fini() override;
    virtual int GetFd() override;
    virtual void DataProcess(std::string _input) override;
    // TCP ͨ���� fd ��Ҫ��д��д�麯��
    std::string ReadFd() final;
    void WriteFd(std::string _output) final;
    // �����û�б�Ҫ��д�� IChannel ʵ���Ϲ�����һ���������麯��ֻΪ�˷��㱩¶�������� �� �Ȳ�д����
    /*virtual void SetNextStage(AZinxHandler* _nextHandler) final;
    virtual AZinxHandler* GetNextStage() final;*/

    // ��Ƶ�ﻹһ����� GetChannelInfo() , Ӧ���Ǽ̳���"IChannel"�� Ŀǰ����ûɶ���Ȳ�д��
    // std::string GetChannelInfo() return "tcp conn on " + m_cfd ��ΪΨһ��ʶ ��
};

class TCPDataChannelFactory
{
public:
    virtual TCPDataChannel* CreateChannel(int _cfd) = 0;
};

class TCPListenChannel :
    public IChannel
{
private:
    TCPDataChannelFactory* m_factory = nullptr;
    unsigned short m_port = 0;
    int m_lfd = -1;

public:
    // _pFact ֱ�� new ���������������ͷ�
    TCPListenChannel(unsigned short _port, TCPDataChannelFactory* _pFact);
    virtual ~TCPListenChannel();

    // ͨ�� IChannel �̳�
    virtual bool Init() override;
    virtual void Fini() override;
    virtual int GetFd() override;
    virtual void DataProcess(std::string _input) override;

    // TCP ͨ���� fd ��Ҫ��д��д�麯��
    // ����������� TCPListenChannel ���� AddChannel �ӵ� kernel �еĻ���ô�� ? 
    // ֱ�ӷ��ؿմ�ò����һ������취, IChannel ��� InternalHandle �����е��� Readfd, �����մ����� nullptr
    // ���� Handle ����Ҳ���ᴦ��
    std::string ReadFd() final;
    void WriteFd(std::string _output) final;


    // ͨ�� IChannel �̳�
    virtual AZinxHandler* GetInputNextStage() override;

    // ��Ƶ�ﻹһ����� GetChannelInfo() , Ӧ���Ǽ̳���"IChannel"�� Ŀǰ����ûɶ���Ȳ�д��
    // std::string GetChannelInfo()
};

