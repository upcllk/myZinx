#pragma once
#include "IChannel.h"
#include <string>

class FifoChannel :
    public IChannel
{
private:
    std::string m_fifoName;
    // �� Init() �����д��ļ��б���ֵ, �ú����ڹ��캯���б�����
    int m_fd = -1;
    // �ùܵ��Ƿ��Ƕ���
    bool m_isRead = true;
    FifoChannel* m_outChannel = nullptr;

public:
    FifoChannel(std::string _file, bool _isRead);
    virtual ~FifoChannel();

    // ͨ�� IChannel �̳�
    virtual int GetFd() override;
    virtual void DataProcess(std::string _input) override;

    // ͨ�� IChannel �̳�
    virtual bool Init() override;
    virtual void Fini() override;

public:
    void SetOutChannel(FifoChannel* _pChannel) {
        m_outChannel = _pChannel;
    }
};

