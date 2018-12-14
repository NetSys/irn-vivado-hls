/* Author: Radhika Mittal
 * File description: Testbench for testing the sender-side modules. Reads from an input trace of event triggers, invokes the corresponding module, and writes the output of the module in the result file.
 */ 

#include "globals.hpp"
#include <stdlib.h>

using namespace hls;
using namespace std;

#define MAXFLOWSIZE 2807 
#define TXFREE 0
#define RECEIVEACK 1
#define TIMEOUT 2

int main(int argc, char *argv[])
{

	ifstream inputFile("../../../../input_trace.txt", ifstream::in);
	ofstream outputFile;
	outputFile.open("output.txt");
	if (!outputFile) {
		cerr << " error opening output file!" << endl;
		return -1;
	}

	//initializing the QP context.
			
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

	//reading number of lines in the input trace.
	int num_lines;
	inputFile >> num_lines;
	
	//reading each subsequent line.
	for (int i = 0; i < num_lines; i++) {

		int eventId;
		int QID;
		inputFile >> QID >> eventId; 

		//if event id corresponds to 'txFree', invoke txFree module with required arguments.
		if(eventId == TXFREE) {
			stream<QPInfoSub1> perQPInfoIn, perQPInfoOut; //input and output streams for QP info
			ap_uint<24> toSend = 0;
			bool sendNew;

			//write relevant QP info to the input stream.
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
			
			//define two other output stream variables that inidicate whether or not a transmit occurs and which packet is to be transmitted.
			stream<bool> sendAllowedStream;
			stream< ap_uint<24> > toSendStream;

			txFree(perQPInfoIn, perQPInfoOut, sendAllowedStream, toSendStream);

			//read the two output stream. 
			sendNew = sendAllowedStream.read();
			toSend = toSendStream.read();
			tempQPInfoSub1 = perQPInfoOut.read();

			//read updated QP info from output stream.
			perQPInfo[QID].nextSNtoSend = tempQPInfoSub1.nextSNtoSend;
			perQPInfo[QID].lastAckedPsn = tempQPInfoSub1.lastAckedPsn;
			perQPInfo[QID].sack_bitmap = tempQPInfoSub1.sack_bitmap;
			perQPInfo[QID].retransmitSN = tempQPInfoSub1.retransmitSN;
			perQPInfo[QID].recoverSN = tempQPInfoSub1.recoverSN;
			perQPInfo[QID].doRetransmit = tempQPInfoSub1.doRetransmit;
			perQPInfo[QID].inRecovery = tempQPInfoSub1.inRecovery;
			perQPInfo[QID].findNewHole = tempQPInfoSub1.findNewHole;
		
			//hack to allow output trace matching with simulation-generated trace for the edge case: when all packets in a flow have been sent, do not update QP info to reflect sending new packet. 
			if((sendNew) && (toSend >= MAXFLOWSIZE)) {
				perQPInfo[QID].nextSNtoSend--;
				sendNew = 0;
				toSend = 0;
			} 

			//write the result in output file.
			QPInfo tempQPInfo = perQPInfo[QID];
			outputFile << TXFREE << " " << sendNew << " " << toSend << " " << tempQPInfo.nextSNtoSend << " " << tempQPInfo.lastAckedPsn << " " << tempQPInfo.retransmitSN << " " << tempQPInfo.recoverSN << " " << tempQPInfo.doRetransmit << " " << tempQPInfo.inRecovery << " " << tempQPInfo.findNewHole << "\n";
		}

		//if event id corresponds to 'receiveAck', invoke receiveAck module with required arguments.
		if (eventId == RECEIVEACK) {
			ap_uint<24> readCumAck, readSackNo;
			bool readAckSyndrome;

			//read ack metadata from input file.
			inputFile >> readCumAck >> readSackNo >> readAckSyndrome;

			stream<QPInfoSub1> perQPInfoAckIn, perQPInfoAckOut;

			//write relevant QP info to the input stream.
			QPInfoSub1 tempQPInfoSub1;
			tempQPInfoSub1.nextSNtoSend = perQPInfo[QID].nextSNtoSend;
			tempQPInfoSub1.lastAckedPsn = perQPInfo[QID].lastAckedPsn;
			tempQPInfoSub1.sack_bitmap = perQPInfo[QID].sack_bitmap;
			tempQPInfoSub1.retransmitSN = perQPInfo[QID].retransmitSN;
			tempQPInfoSub1.recoverSN = perQPInfo[QID].recoverSN;
			tempQPInfoSub1.doRetransmit = perQPInfo[QID].doRetransmit;
			tempQPInfoSub1.inRecovery = perQPInfo[QID].inRecovery;
			tempQPInfoSub1.findNewHole = perQPInfo[QID].findNewHole;
			
			//write the ack info read from file to the input stream.
			AckInfo arrivedAckInfo;
			arrivedAckInfo.cumAck = readCumAck;
			arrivedAckInfo.sackNo = readSackNo;
			arrivedAckInfo.ackSyndrome = readAckSyndrome;
			stream<AckInfo> ackInfoStream;
			ackInfoStream.write(arrivedAckInfo);
			perQPInfoAckIn.write(tempQPInfoSub1);

			receiveAck(ackInfoStream, perQPInfoAckIn, perQPInfoAckOut);

			//read updated QP info from output stream.
			tempQPInfoSub1 = perQPInfoAckOut.read();
			perQPInfo[QID].nextSNtoSend = tempQPInfoSub1.nextSNtoSend;
			perQPInfo[QID].lastAckedPsn = tempQPInfoSub1.lastAckedPsn;
			perQPInfo[QID].sack_bitmap = tempQPInfoSub1.sack_bitmap;
			perQPInfo[QID].retransmitSN = tempQPInfoSub1.retransmitSN;
			perQPInfo[QID].recoverSN = tempQPInfoSub1.recoverSN;
			perQPInfo[QID].doRetransmit = tempQPInfoSub1.doRetransmit;
			perQPInfo[QID].inRecovery = tempQPInfoSub1.inRecovery;
			perQPInfo[QID].findNewHole = tempQPInfoSub1.findNewHole;
		
			//write the result in output file.
			QPInfo tempQPInfo = perQPInfo[QID];
			outputFile << RECEIVEACK << " " << tempQPInfo.nextSNtoSend << " " << tempQPInfo.lastAckedPsn << " " << tempQPInfo.retransmitSN << " " << tempQPInfo.recoverSN << " " << tempQPInfo.doRetransmit << " " << tempQPInfo.inRecovery << " " << tempQPInfo.findNewHole << "\n";
	
		}

		//if event id corresponds to 'timeout', invoke timeout module with required arguments.
		if(eventId == TIMEOUT) {
			bool readQuickTimeout;
			
			//read timeout type from inout file.
			inputFile >> readQuickTimeout;
			perQPInfo[QID].quickTimeout = readQuickTimeout;

			stream<QPInfoSub2> perQPInfoTimeoutIn, perQPInfoTimeoutOut;
			stream<bool> extendStream;
			QPInfoSub2 tempQPInfoSub2;

			//write relevant QP info to the input stream.
			tempQPInfoSub2.nextSNtoSend = perQPInfo[QID].nextSNtoSend;
			tempQPInfoSub2.lastAckedPsn = perQPInfo[QID].lastAckedPsn;
			tempQPInfoSub2.retransmitSN = perQPInfo[QID].retransmitSN;
			tempQPInfoSub2.doRetransmit = perQPInfo[QID].doRetransmit;
			tempQPInfoSub2.findNewHole = perQPInfo[QID].findNewHole;
			tempQPInfoSub2.quickTimeout = perQPInfo[QID].quickTimeout;
			tempQPInfoSub2.rate_factor = perQPInfo[QID].rate_factor;
		
			perQPInfoTimeoutIn.write(tempQPInfoSub2);

			timeout(perQPInfoTimeoutIn, perQPInfoTimeoutOut, extendStream);

			//read extend flag from output stream.
			bool extend = extendStream.read();
			tempQPInfoSub2 = perQPInfoTimeoutOut.read();

			//read updated QP info from output stream.
			perQPInfo[QID].nextSNtoSend = tempQPInfoSub2.nextSNtoSend;
			perQPInfo[QID].lastAckedPsn = tempQPInfoSub2.lastAckedPsn;
			perQPInfo[QID].retransmitSN = tempQPInfoSub2.retransmitSN;
			perQPInfo[QID].doRetransmit = tempQPInfoSub2.doRetransmit;
			perQPInfo[QID].findNewHole = tempQPInfoSub2.findNewHole;
			perQPInfo[QID].quickTimeout = tempQPInfoSub2.quickTimeout;
			perQPInfo[QID].rate_factor = tempQPInfoSub2.rate_factor;
			
			//write the result in output file.
			QPInfo tempQPInfo = perQPInfo[QID];
			outputFile << TIMEOUT << " " << extend << " " << tempQPInfo.nextSNtoSend << " " << tempQPInfo.lastAckedPsn << " " << tempQPInfo.retransmitSN << " " << tempQPInfo.recoverSN << " " << tempQPInfo.doRetransmit << " " << tempQPInfo.inRecovery << " " << tempQPInfo.findNewHole << "\n";
			
		}
	}
	return 0;
}
