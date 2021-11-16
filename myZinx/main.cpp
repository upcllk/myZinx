#include <iostream>
#include <algorithm>
#include <unistd.h>
#include "ZinxKernel.h"
#include "StdinChannel.h"
#include "StdoutChannel.h"
#include "FifoChannel.h"
#include "ProcessFunc.h"
#include "AZinxHandler.h"

using namespace std;

class MessageTest :
    public ZinxMessage
{
public:
    string m_data;
};

class TransUpper :
    public AZinxHandler
{
    // 通过 AZinxHandler 继承
    virtual ZinxMessage* InternelHandle(ZinxMessage* _inputMsg) override
    {
        MessageTest* pMsg = dynamic_cast<MessageTest*>(_inputMsg);
        string outputStr = pMsg->m_data;
        transform(outputStr.begin(), outputStr.end(), outputStr.begin(), ::toupper);
        cout << "TransUpper : " << outputStr << endl;
        // 返回一个堆对象
        MessageTest* pRet = new MessageTest;
        pRet->m_data = outputStr;
        return pRet;
    }
};

class OutputAscii :
    public AZinxHandler
{
    // 通过 AZinxHandler 继承
    virtual ZinxMessage* InternelHandle(ZinxMessage* _inputMsg) override
    {
        MessageTest* pMsg = dynamic_cast<MessageTest*>(_inputMsg);
        if (!pMsg->m_data.empty()) {
            printf("OutputAscii : %d\n", pMsg->m_data[0]);
        }
        // 原样交给下一个环节，但是要 new 一个新的对象
        MessageTest* pRet = new MessageTest;
        pRet->m_data = pMsg->m_data;
        return pRet;
    }
};

class OutputNextLetter :
    public AZinxHandler
{
    // 通过 AZinxHandler 继承
    virtual ZinxMessage* InternelHandle(ZinxMessage* _inputMsg) override
    {
        MessageTest* pMsg = dynamic_cast<MessageTest*>(_inputMsg);
        if (!pMsg->m_data.empty()) {
            printf("OutputNextLetter : %c\n", pMsg->m_data[0] + 1);
        }
        return nullptr;
    }
};


void ChainTest() {
    TransUpper step1;
    OutputAscii step2;
    OutputNextLetter step3;
    step1.SetNextHandler(&step2);
    step2.SetNextHandler(&step3);

    string tmp;
    while (1) {
        getline(cin, tmp);
        if (tmp == "exit") {
            break;
        }
        MessageTest initMsg;
        initMsg.m_data = tmp;
        step1.Handle(&initMsg);
    }
}

int main()
{
    ChainTest();

    ZinxKernel& kernel = ZinxKernel::GetInstance();

    StdinChannel in_channel;
    StdoutChannel out_channel;
    ProcessFunc pf;

    // in_channel -----> pf -----> out_channel
    //           DataProc  DataSend
    in_channel.SetOutProcFunc(&pf);
    pf.SetOutChannel(&out_channel);

    kernel.AddChannel(&out_channel);
    kernel.AddChannel(&in_channel);

    FifoChannel fifo_in_channel("input", true);
    FifoChannel fifo_out_channel("output", false);
    fifo_in_channel.SetOutChannel(&fifo_out_channel);

    kernel.AddChannel(&fifo_in_channel);
    kernel.AddChannel(&fifo_out_channel);

    kernel.Run();
    return 0;
}