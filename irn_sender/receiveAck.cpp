/* Author: Radhika Mittal
 * File description: receiveAck module.
 */

#include "globals.hpp"

void receiveAck(stream<AckInfo> &arrivedAckInfo, stream<QPInfoSub1> &perQPInfoIn, stream<QPInfoSub1> &perQPInfoOut) {
	AckInfo localArrivedAckInfo = arrivedAckInfo.read();
	QPInfoSub1 localPerQPInfo = perQPInfoIn.read();

	//update next sequence to send, if cumulative ack is higher.
	if(localArrivedAckInfo.cumAck > localPerQPInfo.nextSNtoSend)
		localPerQPInfo.nextSNtoSend = localArrivedAckInfo.cumAck;
	
	//check if any new packets are cumulatively acked and update last ack value and the bitmap. 
	bool newAck = false;
	if(localArrivedAckInfo.cumAck > localPerQPInfo.lastAckedPsn) {
		newAck = true;
		localPerQPInfo.sack_bitmap = localPerQPInfo.sack_bitmap >> (localArrivedAckInfo.cumAck - localPerQPInfo.lastAckedPsn);
		localPerQPInfo.lastAckedPsn = localArrivedAckInfo.cumAck;
	}

	//if a nack is received, 
	if(localArrivedAckInfo.ackSyndrome) {
		// update bitmap to set selective ack.
		ap_uint<BDP> temp = 1;
		temp = temp << (localArrivedAckInfo.sackNo - localPerQPInfo.lastAckedPsn);
		localPerQPInfo.sack_bitmap = localPerQPInfo.sack_bitmap | temp;
		if(localPerQPInfo.inRecovery) {
			//if packet set for retransmission is selectively acked, don't retransmit it, and find new hole instead.
			if(localPerQPInfo.doRetransmit && (localPerQPInfo.retransmitSN == localArrivedAckInfo.sackNo)) {
				localPerQPInfo.findNewHole = true;
				localPerQPInfo.doRetransmit = false;
			}
			//if the sack creation leads to new holes, and retransmit flag is not set to true, 
			//set flag to find new holes.
			else if(!localPerQPInfo.doRetransmit) {
				ap_uint<BDP> temp2 = localPerQPInfo.sack_bitmap >> (localArrivedAckInfo.sackNo - localPerQPInfo.lastAckedPsn + 1);
				if(temp2 == 0) {
					localPerQPInfo.findNewHole = true;
				}
			} 
		}
	}

	//when cumulative ack is greater than recovery sequence, 
	//exit recovery and disable flags for retransmission and bitmap lookup.
	if (localArrivedAckInfo.cumAck > localPerQPInfo.recoverSN) {
		localPerQPInfo.inRecovery = false;
		localPerQPInfo.findNewHole = false;
		localPerQPInfo.doRetransmit = false;
	}

	//if new packets have been acked,
	if (newAck) {
		if(localPerQPInfo.inRecovery) {
			//partial ack: retransmit this packet only if it has not already been retransmitted. 
			if((localPerQPInfo.retransmitSN < localPerQPInfo.lastAckedPsn) || ((localPerQPInfo.retransmitSN == localPerQPInfo.lastAckedPsn) && (localPerQPInfo.doRetransmit))) { 
				localPerQPInfo.retransmitSN = localPerQPInfo.lastAckedPsn;
				localPerQPInfo.doRetransmit = true;
				localPerQPInfo.findNewHole = false;
			}	 
		}	 
	} else {
		if(!localPerQPInfo.inRecovery) {
			//first duplicated cumulative ack. 
			//check if it is due to a valid lost packet.
			if ((localPerQPInfo.lastAckedPsn < localPerQPInfo.nextSNtoSend) && (localArrivedAckInfo.ackSyndrome)) { 
				localPerQPInfo.retransmitSN = localPerQPInfo.lastAckedPsn;
				localPerQPInfo.doRetransmit = true;
				localPerQPInfo.findNewHole = false;
			}
		}
	}
	
	perQPInfoOut.write(localPerQPInfo);
}

