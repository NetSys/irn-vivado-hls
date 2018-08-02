#include "globals.hpp"


inline bool findNextHoleTx(ap_uint<BDP> &bitmap, ap_uint<LOGBDP> &nextHole) {
	#pragma HLS INLINE
	ap_uint<LOGBDP> bits_to_shiftArr[BDPBY32];
	ap_uint<LOGBDP> bits_to_shift = 0;

	collectPartStats: for(int i=0; i < BDPBY32; i++) {
	#pragma HLS unroll
		int idx = i << 5;
		ap_uint<32> part = bitmap.range(idx + 31, idx);;
		ap_uint<LOGBDP> bits_to_shift_part = 0;
		if((part & 0xffff) == 0xffff) {
			bits_to_shift_part += 16;
			part = part >> 16;
		}
		if((part & 0xff) == 0xff) {
			bits_to_shift_part += 8;
			part = part >> 8;
		}
		if((part & 0xf) == 0xf) {
			bits_to_shift_part += 4;
			part = part >> 4;
		}
		if((part & 3) == 3) {
			bits_to_shift_part += 2;
			part = part >> 2;
		}
		if((part & 1) == 1){
			bits_to_shift_part += 1;
			part = part >> 1;
		}
		if((part & 1) == 1){
			bits_to_shift_part += 1;
		}
		bits_to_shiftArr[i] = bits_to_shift_part;

		bool factor = 1;
		if(i > 0)
			factor = bool(ap_uint<LOGBDP>(bits_to_shiftArr[i - 1] >> 5));
		bits_to_shiftArr[i] *= factor;
		bits_to_shift += bits_to_shiftArr[i];
	}
  bitmap = bitmap >> bits_to_shift;
	nextHole = bits_to_shift;
	if(bitmap == 0) return 0;
	return 1; 
} 



void txFree(stream<QPInfoSub1> &perQPInfoIn, stream<QPInfoSub1> &perQPInfoOut, stream<bool> &sendAllowedStream, stream< ap_uint<24> > &toSendStream) {
	//#pragma HLS pipeline
	#pragma HLS STREAM variable=perQPInfoIn 
	#pragma HLS STREAM variable=perQPInfoOut
	#pragma HLS STREAM variable=sendAllowedStream
	#pragma HLS STREAM variable=toSendStream
  static ap_uint<24> maxCap = MAXCAP;
	QPInfoSub1 localPerQPInfo = perQPInfoIn.read();
	ap_uint<24> toSend = 0;
  bool sendNew = false;
  if((localPerQPInfo.doRetransmit) && (localPerQPInfo.retransmitSN >= localPerQPInfo.lastAckedPsn)) {
      if(localPerQPInfo.retransmitSN == localPerQPInfo.lastAckedPsn) localPerQPInfo.inRecovery = true;
      toSend = localPerQPInfo.retransmitSN;
      sendNew = true;
      localPerQPInfo.recoverSN = localPerQPInfo.nextSNtoSend - 1;
      localPerQPInfo.doRetransmit = false;
			localPerQPInfo.findNewHole = true;
  } else {
		if(localPerQPInfo.doRetransmit) localPerQPInfo.findNewHole = false;
    localPerQPInfo.doRetransmit = false;
		if(localPerQPInfo.nextSNtoSend - localPerQPInfo.lastAckedPsn < maxCap) {
			toSend = localPerQPInfo.nextSNtoSend;
			sendNew = true;
			localPerQPInfo.nextSNtoSend = localPerQPInfo.nextSNtoSend + 1;
		} 
  }

	if(localPerQPInfo.findNewHole) {
		ap_uint<BDP> tempBitmap;
		ap_uint<24> startidx;
		if(localPerQPInfo.retransmitSN >= localPerQPInfo.lastAckedPsn) {
			tempBitmap = localPerQPInfo.sack_bitmap >> (localPerQPInfo.retransmitSN - localPerQPInfo.lastAckedPsn + 1);
			startidx = localPerQPInfo.retransmitSN + 1;
		} else {
			tempBitmap = localPerQPInfo.sack_bitmap;
			startidx = localPerQPInfo.lastAckedPsn;
		}
		ap_uint<LOGBDP> nextHole = 0;
		bool holeFound;
		holeFound = findNextHoleTx(tempBitmap, nextHole);
		if(holeFound) {
			localPerQPInfo.retransmitSN = nextHole + startidx;
			localPerQPInfo.doRetransmit = true;
		} 
		localPerQPInfo.findNewHole = false;
	}

	perQPInfoOut.write(localPerQPInfo);
	sendAllowedStream.write(sendNew);
	toSendStream.write(toSend);
}

