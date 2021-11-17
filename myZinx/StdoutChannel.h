#pragma once
#include "IChannel.h"

class StdoutChannel :
    public IChannel
{
public:
    StdoutChannel();
    virtual ~StdoutChannel();

    // 通过 IChannel 继承
    virtual int GetFd() override;

    // 通过 IChannel 继承
    virtual bool Init() override;
    virtual void Fini() override;

    // 通过 IChannel 继承
    virtual void DataProcess(std::string _input) override;
};

