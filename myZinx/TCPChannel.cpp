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
    // ���ΪɶҪ��ô�����ͳ������õ� needclose �� kernel �е�ʵ���й� ? ��Ƶ������ȫû����
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
    // ѭ�����Ļ��������������� -> ���÷�������
    while (1) {
        int recvLen =  recv(m_cfd, buffer, sizeof(buffer), MSG_DONTWAIT);
        if (recvLen == 0) {
            // ���Ӷ���
            /*
            * �и�ÿһ�� channel ���е� setchannelclose ����������������� m_needclose Ϊtrue
            * ����� kernel run ѭ���б���׽��
            * ����������汾��ûд�Ȼ���˵��
            * ������ʱ����û���� ������ 
            */
            retStr.clear();
            break;
        }
        else if (recvLen == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                //����û�ϣ�ֻ��û������
                break;
            }
            else {
                // ���󣬴���ʽͬ���Ӷ���
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
        // ���������ظ���
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
