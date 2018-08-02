############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
############################################################
open_project -reset rdma_receiver
set_top receiveData
add_files receiveData.cpp
add_files -tb receiveData_tb.cpp
open_solution -reset "temp_solution"
set_part {xcku060-ffva1156-2-i}
create_clock -period 6.3
csim_design
csynth_design
cosim_design
exit
