#include "globals.hpp"

void receiveAck(stream<AckInfo> &arrivedAckInfo, stream<QPInfoSub1> &perQPInfoIn, stream<QPInfoSub1> &perQPInfoOut) {
	//#pragma HLS pipeline

	AckInfo localArrivedAckInfo = arrivedAckInfo.read();
	QPInfoSub1 localPerQPInfo = perQPInfoIn.read();

	if(localArrivedAckInfo.cumAck > localPerQPInfo.nextSNtoSend) localPerQPInfo.nextSNtoSend = localArrivedAckInfo.cumAck;
	

	bool newAck = false;
	if(localArrivedAckInfo.cumAck > localPerQPInfo.lastAckedPsn) {
		newAck = true;
		localPerQPInfo.sack_bitmap = localPerQPInfo.sack_bitmap >> (localArrivedAckInfo.cumAck - localPerQPInfo.lastAckedPsn);
		localPerQPInfo.lastAckedPsn = localArrivedAckInfo.cumAck;
	}

	if(localArrivedAckInfo.ackSyndrome) {
		ap_uint<BDP> temp = 1;
		temp = temp << (localArrivedAckInfo.sackNo - localPerQPInfo.lastAckedPsn);
		localPerQPInfo.sack_bitmap = localPerQPInfo.sack_bitmap | temp;
		if(localPerQPInfo.inRecovery) {
			if(localPerQPInfo.doRetransmit && (localPerQPInfo.retransmitSN == localArrivedAckInfo.sackNo)) {
				localPerQPInfo.findNewHole = true;
				localPerQPInfo.doRetransmit = false;
			}
			else if(!localPerQPInfo.doRetransmit) {
				ap_uint<BDP> temp2 = localPerQPInfo.sack_bitmap >> (localArrivedAckInfo.sackNo - localPerQPInfo.lastAckedPsn + 1);
				if(temp2 == 0) {
					localPerQPInfo.findNewHole = true;
				}
			} 
		}
	}

	if (localArrivedAckInfo.cumAck > localPerQPInfo.recoverSN) {
		localPerQPInfo.inRecovery = false;
		localPerQPInfo.findNewHole = false;
		localPerQPInfo.doRetransmit = false;
	}

  if (newAck) {
    if(localPerQPInfo.inRecovery) {
      //partial ack
      if((localPerQPInfo.retransmitSN < localPerQPInfo.lastAckedPsn) || ((localPerQPInfo.retransmitSN == localPerQPInfo.lastAckedPsn) && (localPerQPInfo.doRetransmit))) { //retransmit this packet only if it has not already been retransmitted. 
        localPerQPInfo.retransmitSN = localPerQPInfo.lastAckedPsn;
        localPerQPInfo.doRetransmit = true;
				localPerQPInfo.findNewHole = false;
      }   
    }   
  } else {
    if(!localPerQPInfo.inRecovery) {
      // first dupack
      if ((localPerQPInfo.lastAckedPsn < localPerQPInfo.nextSNtoSend) && (localArrivedAckInfo.ackSyndrome)) { //dup ack due to valid lost packet
        localPerQPInfo.retransmitSN = localPerQPInfo.lastAckedPsn;
        localPerQPInfo.doRetransmit = true;
				localPerQPInfo.findNewHole = false;
      }
    }
  }
	
	perQPInfoOut.write(localPerQPInfo);
}

