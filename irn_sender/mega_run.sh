#add source and licence information below, and uncomment the lines.
#source "path to Vivado installation"/settings64.sh
#export XILINXD_LICENSE_FILE="source of license file"

for i in 128
do
	for func in txFree receiveAck timeout
	do
		cp bitmapSize-$i.hpp bitmapSize.hpp
		cp traces/sendInput-40Gbps-incast.txt input_trace.txt
		vivado_hls run_hls_$func.tcl
		rm -rf rdma_sender_$func/solution-$i
		mkdir rdma_sender_$func/solution-$i
		mv rdma_sender_$func/temp_solution/* rdma_sender_$func/solution-$i/
	done
done
	
