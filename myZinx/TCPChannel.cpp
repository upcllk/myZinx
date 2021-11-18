#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "TCPChannel.h"
#include "ZinxKernel.h"

TCPDataChannel::TCPDataChannel(int _cfd)
    :m_cfd(_cfd)
{
}

TCPDataChannel::~TCPDataChannel()
{
    if (m_cfd >= 0) {
        close(m_cfd);
        m_cfd = -1;
    }
}

bool TCPDataChannel::Init()
{
    return true;
}

void TCPDataChannel::Fini()
{
    // 这边为啥要这么做，和出错设置的 needclose 在 kernel 中的实现有关 ? 视频声音不全没法看
    if (m_cfd >= 0) {
        close(m_cfd);
        m_cfd = -1;
    }
}

int TCPDataChannel::GetFd()
{
    return m_cfd;
}

void TCPDataChannel::DataProcess(std::string _input)
{
}

std::string TCPDataChannel::ReadFd()
{
    std::string retStr;
    char buffer[BUFSIZ];
    memset(buffer, 0, sizeof(buffer));
    // 循环读的话可能阻塞在这里 -> 设置非阻塞读
    while (1) {
        int recvLen =  recv(m_cfd, buffer, sizeof(buffer), MSG_DONTWAIT);
        if (recvLen == 0) {
            // 连接断了
            /*
            * 有个每一个 channel 都有的 setchannelclose 函数，这个函数设置 m_needclose 为true
            * 这个在 kernel run 循环中被捕捉到
            * 由于我这个版本还没写等会再说吧
            * 可以暂时假设没错误 。。。 
            */
            retStr.clear();
            break;
        }
        else if (recvLen == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                //连接没断，只是没数据了
                break;
            }
            else {
                // 错误，处理方式同连接断了
                retStr.clear();
                break;
            }
        }
        else {
            retStr.append(buffer, recvLen);
        }
    }
    
    return retStr;
}

void TCPDataChannel::WriteFd(std::string _output)
{
    send(m_cfd, _output.data(), _output.size(), 0);
}

TCPListenChannel::TCPListenChannel(unsigned short _port, TCPDataChannelFactory* _pFact)
    :m_port(_port), m_factory(_pFact)
{
}

TCPListenChannel::~TCPListenChannel()
{
    if (m_factory != nullptr) {
        delete m_factory;
    }
}

bool TCPListenChannel::Init()
{
    bool bRet = false;
    int lfd = -1;
    lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd >= 0) {
        struct sockaddr_in servAddr;
        servAddr.sin_family = AF_INET;
        servAddr.sin_addr.s_addr = INADDR_ANY;
        servAddr.sin_port = htons(m_port);
        // 设置允许重复绑定
        int flag = 1;
        setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &flag, (socklen_t)sizeof(flag));

        if (bind(lfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) == 0) {
            if (listen(lfd, 1000) == 0) {
                bRet = true;
                m_lfd = lfd;
            }
            else {
                perror(__FILE__":""__FUNC__"":""listen error");
            }
        }
        else {
            perror(__FILE__":""__FUNC__"":""bind error");
        }
    }
    else {
        perror(__FILE__":""__FUNC__"":""socket error");
    }

    return bRet;
}

void TCPListenChannel::Fini()
{
    if (m_lfd >= 0) {
        close(m_lfd);
    }
}

int TCPListenChannel::GetFd()
{
    return m_lfd;
}

void TCPListenChannel::DataProcess(std::string _input)
{
}

std::string TCPListenChannel::ReadFd()
{
    bool bRet = false;
    struct sockaddr_in clitAddr;
    socklen_t clitAddrLen = sizeof(clitAddr);
    int cfd = accept(m_lfd, (struct sockaddr*)&clitAddr, &clitAddrLen);
    if (cfd >= 0) {
        TCPDataChannel* pTCPDataChannel = m_factory->CreateChannel(cfd);
        ZinxKernel::GetInstance().AddChannel(pTCPDataChannel);
    }
    else {
        perror(__FILE__":""__FUNC__"":""accept error");
    }

    return std::string();
}

void TCPListenChannel::WriteFd(std::string _output)
{
}

AZinxHandler* TCPListenChannel::GetInputNextStage()
{
    return nullptr;
}
