source /data/xilinx/Vivado/2017.2/settings64.sh
export XILINXD_LICENSE_FILE="source of file"

touch txFree_report_summary
touch receiveAck_report_summary
touch timeout_report_summary

for i in 128
do
	for func in txFree receiveAck timeout
	do
		cp bitmapSize-$i.hpp bitmapSize.hpp
		vivado_hls run_hls_$func.tcl
		rm -rf rdma_sender_$func/solution-$i
		mkdir rdma_sender_$func/solution-$i
		mv rdma_sender_$func/temp_solution/* rdma_sender_$func/solution-$i/
	done
done
	
