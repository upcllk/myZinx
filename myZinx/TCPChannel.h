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

    // 通过 IChannel 继承
    virtual bool Init() override;
    virtual void Fini() override;
    virtual int GetFd() override;
    virtual void DataProcess(std::string _input) override;
    // TCP 通道的 fd 需要重写读写虚函数
    std::string ReadFd() final;
    void WriteFd(std::string _output) final;
    // 这边有没有必要重写和 IChannel 实际上功能是一样的两个虚函数只为了方便暴露给调用者 ？ 先不写看看
    /*virtual void SetNextStage(AZinxHandler* _nextHandler) final;
    virtual AZinxHandler* GetNextStage() final;*/

    // 视频里还一个多的 GetChannelInfo() , 应该是继承于"IChannel"的 目前好像没啥用先不写了
    // std::string GetChannelInfo() return "tcp conn on " + m_cfd 作为唯一标识 ？
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
    // _pFact 直接 new 出来，析构函数释放
    TCPListenChannel(unsigned short _port, TCPDataChannelFactory* _pFact);
    virtual ~TCPListenChannel();

    // 通过 IChannel 继承
    virtual bool Init() override;
    virtual void Fini() override;
    virtual int GetFd() override;
    virtual void DataProcess(std::string _input) override;

    // TCP 通道的 fd 需要重写读写虚函数
    // 这个如果后面把 TCPListenChannel 调用 AddChannel 加到 kernel 中的话怎么做 ? 
    // 直接返回空串貌似是一个解决办法, IChannel 里的 InternalHandle 函数中调用 Readfd, 读到空串返回 nullptr
    // 这样 Handle 函数也不会处理
    std::string ReadFd() final;
    void WriteFd(std::string _output) final;


    // 通过 IChannel 继承
    virtual AZinxHandler* GetInputNextStage() override;

    // 视频里还一个多的 GetChannelInfo() , 应该是继承于"IChannel"的 目前好像没啥用先不写了
    // std::string GetChannelInfo()
};

