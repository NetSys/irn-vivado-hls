## IRN's Vivado HLS Code

This repository contains the Xilinx Vivado HLS code for synthesizing IRN's packet processing logic.

#### Contents of the Repository

There are two folders in the repository:

**irn_receiver**: This folder contains the code for the receiver side module, namely *receiveData*.

**irn_sender**: This folder contains the code for sender side modules, namely *txFree*, *receiveAck* and *timeout*.

Each folder contains scripts to run synthesis, RTL verification and exporting for each module. A sample trace for input to the respective modules and the output, generated via simulations, has also been provided for the verification. 

#### Usage

The code assumes availability to Xilinx Vivado HLS 2017.2 (or any other HLS version that supports the target device Kintex Ultrascale XCKU060).

###### IRN Receiver

1. Go to *irn_receiver* folder.

2. Update the file 'megarun.sh' to set the correct path to the licence file. 

3. Run 'megarun.sh'.

4. The synthesis report can be found in *rdma_receiver/solution-128/syn/report/receiveData_csynth.rpt*.  

5. The trace output can be found at *rdma_receiver/solution-128/sim/wrapc/output.txt* and can be compared against *traces/recvOutput-40Gbps-incast.txt*

###### IRN Sender

1. Go to *irn_sender* folder.

2. Update the file 'megarun.sh' to set the correct path to the licence file. 

3. Run 'megarun.sh'.

4. For each *$module* in *(txFree, receiveAck, timeout)*, the synthesis report can be found in *rdma_sender_$module/solution-128/syn/report/$module_csynth.rpt*.  

5. The trace output can be found at *rdma_sender_$module/solution-128/sim/wrapc/output.txt* and can be compared against *traces/sendOutput-40Gbps-incast.txt* (after concatenating part1 and part2 of the latter).
