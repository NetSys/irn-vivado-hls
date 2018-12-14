/* Author: Radhika Mittal
 * File description: timeout module.
 */

#include "globals.hpp"

void timeout(stream<QPInfoSub2> &perQPInfoIn, stream<QPInfoSub2> &perQPInfoOut, stream<bool> &timeoutExtend) {
	//#pragma HLS pipeline
	static ap_uint<24> quickTimeoutCap = 3;
	static ap_uint<24> rate_factor_limit = 4;
	
	QPInfoSub2 localPerQPInfo = perQPInfoIn.read();
	bool localTimeoutExtend = false;

	//if the timeout was triggered via quickTimeout (i.e. using RTO_low value), 
	//check if condition for quick timeout holds. 
	//if the condition does not hold, set the 'extend' flag.
	//if timeout is valid, mark the last acked packet for retransmission.
	if(localPerQPInfo.quickTimeout && ((localPerQPInfo.rate_factor > rate_factor_limit) || ((localPerQPInfo.nextSNtoSend - localPerQPInfo.lastAckedPsn) > quickTimeoutCap))) {
		localTimeoutExtend = true;
	} else {
		localPerQPInfo.retransmitSN = localPerQPInfo.lastAckedPsn;
		localPerQPInfo.doRetransmit = true;
		localPerQPInfo.findNewHole = false;
	}
	//write the output.
	timeoutExtend.write(localTimeoutExtend);
	perQPInfoOut.write(localPerQPInfo);
}

