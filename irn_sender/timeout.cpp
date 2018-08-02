#include "globals.hpp"

void timeout(stream<QPInfoSub2> &perQPInfoIn, stream<QPInfoSub2> &perQPInfoOut, stream<bool> &timeoutExtend) {
	//#pragma HLS pipeline
	static ap_uint<24> quickTimeoutCap = 3;
	static ap_uint<24> rate_factor_limit = 4;
	
	QPInfoSub2 localPerQPInfo = perQPInfoIn.read();
	bool localTimeoutExtend = false;
	if(localPerQPInfo.quickTimeout && ((localPerQPInfo.rate_factor > rate_factor_limit) || ((localPerQPInfo.nextSNtoSend - localPerQPInfo.lastAckedPsn) > quickTimeoutCap))) {
		localTimeoutExtend = true;
	} else {
		localPerQPInfo.retransmitSN = localPerQPInfo.lastAckedPsn;
		localPerQPInfo.doRetransmit = true;
		localPerQPInfo.findNewHole = false;
	}
	timeoutExtend.write(localTimeoutExtend);
	perQPInfoOut.write(localPerQPInfo);
}

