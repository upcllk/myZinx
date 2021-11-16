#pragma once
#include "IChannel.h"
class StdoutChannel :
    public IChannel
{
public:
    StdoutChannel();
    virtual ~StdoutChannel();

    // ͨ�� IChannel �̳�
    virtual std::string ReadFd() override;
    virtual int GetFd() override;
    virtual void DataProcess(std::string _input) override;
};

