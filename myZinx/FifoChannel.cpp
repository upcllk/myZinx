#include "FifoChannel.h"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

FifoChannel::FifoChannel(std::string _file, bool _isRead)
    : m_fifoName(_file), m_isRead(_isRead)
{
}

FifoChannel::~FifoChannel()
{
}

int FifoChannel::GetFd()
{
    return m_fd;
}

void FifoChannel::DataProcess(std::string _input)
{
    if(m_outChannel!=nullptr){
        m_outChannel->DataSendOut(_input);
    }
}

bool FifoChannel::Init()
{
    bool bRet = false;
    // 打开文件
    int flag = O_RDONLY;
    if (m_isRead == false) {
        flag = O_WRONLY;
    }
    int fd = open(m_fifoName.c_str(), flag | O_NONBLOCK);
    if (fd >= 0) {
        m_fd = fd;
        bRet = true;
    }
    // std::cout << (bRet ? "success" : "fail") << std::endl;
    return bRet;
}

void FifoChannel::Fini()
{
    if (m_fd >= 0) {
        close(m_fd);
    }
}
