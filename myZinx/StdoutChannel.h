#pragma once
#include "IChannel.h"
class StdoutChannel :
    public IChannel
{
public:
    StdoutChannel();
    virtual ~StdoutChannel();

    // Í¨¹ý IChannel ¼Ì³Ð
    virtual int GetFd() override;
    virtual void DataProcess(std::string _input) override;
};

