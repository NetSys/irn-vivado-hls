source /data/xilinx/Vivado/2017.2/settings64.sh
export XILINXD_LICENSE_FILE="source of license file"

touch recvData_report_summary


for i in 128
do
	cp traces/recvInput-40Gbps-incast.txt input_trace.txt
	cp bitmapSize-$i.hpp bitmapSize.hpp
	vivado_hls run_hls.tcl
	rm -rf rdma_receiver/solution-$i
	mkdir rdma_receiver/solution-$i
	mv rdma_receiver/temp_solution/* rdma_receiver/solution-$i/
done

