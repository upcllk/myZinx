#pragma once
#include "IChannel.h"
#include <string>

class FifoChannel :
    public IChannel
{
private:
    std::string m_fifoName;
    // 在 Init() 函数中打开文件中被赋值, 该函数在构造函数中被调用
    int m_fd = -1;
    // 该管道是否是读端
    bool m_isRead = true;
    FifoChannel* m_outChannel = nullptr;

public:
    FifoChannel(std::string _file, bool _isRead);
    virtual ~FifoChannel();

    // 通过 IChannel 继承
    virtual int GetFd() override;
    virtual void DataProcess(std::string _input) override;

    // 通过 IChannel 继承
    virtual bool Init() override;
    virtual void Fini() override;

public:
    void SetOutChannel(FifoChannel* _pChannel) {
        m_outChannel = _pChannel;
    }
};

