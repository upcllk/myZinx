﻿#include <iostream>
#include "ZinxKernel.h"
#include "StdinChannel.h"
#include "StdoutChannel.h"
#include "FifoChannel.h"
#include "ProcessFunc.h"

int main()
{
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