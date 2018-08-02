#include "globals.hpp"
#include <stdlib.h>

using namespace hls;
using namespace std;

#define MAXFLOWSIZE 2807 

int main(int argc, char *argv[])
{

	ifstream inputFile("../../../../input_trace.txt", ifstream::in);
	ofstream outputFile;
	outputFile.open("output.txt");
	if (!outputFile) {
		cerr << " error opening output file!" << endl;
		return -1;
	}

		
	QPInfo perQPInfo[N];

	for(int i = 0; i < N; i++) {
		perQPInfo[i].nextSNtoSend = 0;
		perQPInfo[i].lastAckedPsn = 0;
		perQPInfo[i].sack_bitmap = 0;
		perQPInfo[i].retransmitSN = 0;
  	perQPInfo[i].rate_factor = 1;
		perQPInfo[i].recoverSN = 0;
		perQPInfo[i].doRetransmit = 0;
		perQPInfo[i].inRecovery = 0;
		perQPInfo[i].quickTimeout = 1;
	}

	int num_lines;
	inputFile >> num_lines;
	
	for (int i = 0; i < num_lines; i++) {
		int eventId;
		int QID;
		inputFile >> QID >> eventId; 
		if(eventId == 0) {
			stream<QPInfoSub1> perQPInfoIn, perQPInfoOut;
			ap_uint<24> toSend = 0;
			bool sendNew;

			QPInfoSub1 tempQPInfoSub1;
			tempQPInfoSub1.nextSNtoSend = perQPInfo[QID].nextSNtoSend;
			tempQPInfoSub1.lastAckedPsn = perQPInfo[QID].lastAckedPsn;
			tempQPInfoSub1.sack_bitmap = perQPInfo[QID].sack_bitmap;
			tempQPInfoSub1.retransmitSN = perQPInfo[QID].retransmitSN;
			tempQPInfoSub1.recoverSN = perQPInfo[QID].recoverSN;
			tempQPInfoSub1.doRetransmit = perQPInfo[QID].doRetransmit;
			tempQPInfoSub1.inRecovery = perQPInfo[QID].inRecovery;
			tempQPInfoSub1.findNewHole = perQPInfo[QID].findNewHole;
			perQPInfoIn.write(tempQPInfoSub1);
			
			stream<bool> sendAllowedStream;
			stream< ap_uint<24> > toSendStream;

			txFree(perQPInfoIn, perQPInfoOut, sendAllowedStream, toSendStream);

			sendNew = sendAllowedStream.read();
			toSend = toSendStream.read();
			tempQPInfoSub1 = perQPInfoOut.read();

			perQPInfo[QID].nextSNtoSend = tempQPInfoSub1.nextSNtoSend;
			perQPInfo[QID].lastAckedPsn = tempQPInfoSub1.lastAckedPsn;
			perQPInfo[QID].sack_bitmap = tempQPInfoSub1.sack_bitmap;
			perQPInfo[QID].retransmitSN = tempQPInfoSub1.retransmitSN;
			perQPInfo[QID].recoverSN = tempQPInfoSub1.recoverSN;
			perQPInfo[QID].doRetransmit = tempQPInfoSub1.doRetransmit;
			perQPInfo[QID].inRecovery = tempQPInfoSub1.inRecovery;
			perQPInfo[QID].findNewHole = tempQPInfoSub1.findNewHole;
		
			if((sendNew) && (toSend >= MAXFLOWSIZE)) {
				perQPInfo[QID].nextSNtoSend--;
				sendNew = 0;
				toSend = 0;
			} 
			QPInfo tempQPInfo = perQPInfo[QID];
			outputFile << "0 " << sendNew << " " << toSend << " " << tempQPInfo.nextSNtoSend << " " << tempQPInfo.lastAckedPsn << " " << tempQPInfo.retransmitSN << " " << tempQPInfo.recoverSN << " " << tempQPInfo.doRetransmit << " " << tempQPInfo.inRecovery << " " << tempQPInfo.findNewHole << "\n";
		}
		if (eventId == 1) {
			ap_uint<24> readCumAck, readSackNo;
			bool readAckSyndrome;

			inputFile >> readCumAck >> readSackNo >> readAckSyndrome;

			stream<QPInfoSub1> perQPInfoAckIn, perQPInfoAckOut;
			QPInfoSub1 tempQPInfoSub1;
			tempQPInfoSub1.nextSNtoSend = perQPInfo[QID].nextSNtoSend;
			tempQPInfoSub1.lastAckedPsn = perQPInfo[QID].lastAckedPsn;
			tempQPInfoSub1.sack_bitmap = perQPInfo[QID].sack_bitmap;
			tempQPInfoSub1.retransmitSN = perQPInfo[QID].retransmitSN;
			tempQPInfoSub1.recoverSN = perQPInfo[QID].recoverSN;
			tempQPInfoSub1.doRetransmit = perQPInfo[QID].doRetransmit;
			tempQPInfoSub1.inRecovery = perQPInfo[QID].inRecovery;
			tempQPInfoSub1.findNewHole = perQPInfo[QID].findNewHole;
			
			AckInfo arrivedAckInfo;
			arrivedAckInfo.cumAck = readCumAck;
			arrivedAckInfo.sackNo = readSackNo;
			arrivedAckInfo.ackSyndrome = readAckSyndrome;
			stream<AckInfo> ackInfoStream;
			ackInfoStream.write(arrivedAckInfo);
			perQPInfoAckIn.write(tempQPInfoSub1);

			receiveAck(ackInfoStream, perQPInfoAckIn, perQPInfoAckOut);

			tempQPInfoSub1 = perQPInfoAckOut.read();
			perQPInfo[QID].nextSNtoSend = tempQPInfoSub1.nextSNtoSend;
			perQPInfo[QID].lastAckedPsn = tempQPInfoSub1.lastAckedPsn;
			perQPInfo[QID].sack_bitmap = tempQPInfoSub1.sack_bitmap;
			perQPInfo[QID].retransmitSN = tempQPInfoSub1.retransmitSN;
			perQPInfo[QID].recoverSN = tempQPInfoSub1.recoverSN;
			perQPInfo[QID].doRetransmit = tempQPInfoSub1.doRetransmit;
			perQPInfo[QID].inRecovery = tempQPInfoSub1.inRecovery;
			perQPInfo[QID].findNewHole = tempQPInfoSub1.findNewHole;
		
			QPInfo tempQPInfo = perQPInfo[QID];
			outputFile << "1" << " " << tempQPInfo.nextSNtoSend << " " << tempQPInfo.lastAckedPsn << " " << tempQPInfo.retransmitSN << " " << tempQPInfo.recoverSN << " " << tempQPInfo.doRetransmit << " " << tempQPInfo.inRecovery << " " << tempQPInfo.findNewHole << "\n";
	
			}
			if(eventId == 2) {
				bool readQuickTimeout;
				inputFile >> readQuickTimeout;
				perQPInfo[QID].quickTimeout = readQuickTimeout;

				stream<QPInfoSub2> perQPInfoTimeoutIn, perQPInfoTimeoutOut;
				stream<bool> extendStream;
				QPInfoSub2 tempQPInfoSub2;
				tempQPInfoSub2.nextSNtoSend = perQPInfo[QID].nextSNtoSend;
				tempQPInfoSub2.lastAckedPsn = perQPInfo[QID].lastAckedPsn;
				tempQPInfoSub2.retransmitSN = perQPInfo[QID].retransmitSN;
				tempQPInfoSub2.doRetransmit = perQPInfo[QID].doRetransmit;
				tempQPInfoSub2.findNewHole = perQPInfo[QID].findNewHole;
				tempQPInfoSub2.quickTimeout = perQPInfo[QID].quickTimeout;
				tempQPInfoSub2.rate_factor = perQPInfo[QID].rate_factor;
			
				perQPInfoTimeoutIn.write(tempQPInfoSub2);
				timeout(perQPInfoTimeoutIn, perQPInfoTimeoutOut, extendStream);

				bool extend = extendStream.read();
				tempQPInfoSub2 = perQPInfoTimeoutOut.read();

				perQPInfo[QID].nextSNtoSend = tempQPInfoSub2.nextSNtoSend;
				perQPInfo[QID].lastAckedPsn = tempQPInfoSub2.lastAckedPsn;
				perQPInfo[QID].retransmitSN = tempQPInfoSub2.retransmitSN;
				perQPInfo[QID].doRetransmit = tempQPInfoSub2.doRetransmit;
				perQPInfo[QID].findNewHole = tempQPInfoSub2.findNewHole;
				perQPInfo[QID].quickTimeout = tempQPInfoSub2.quickTimeout;
				perQPInfo[QID].rate_factor = tempQPInfoSub2.rate_factor;
				
				QPInfo tempQPInfo = perQPInfo[QID];
				outputFile << "2" << " " << extend << " " << tempQPInfo.nextSNtoSend << " " << tempQPInfo.lastAckedPsn << " " << tempQPInfo.retransmitSN << " " << tempQPInfo.recoverSN << " " << tempQPInfo.doRetransmit << " " << tempQPInfo.inRecovery << " " << tempQPInfo.findNewHole << "\n";
				
			}
	}
	return 0;
}
