/* Author: Radhika Mittal
 * File description: Testbench for testing the receiver-side module. Reads from an input trace of event triggers, invokes the receiveData module, and writes the output of the module in the result file.
 */ 

#include "globals.hpp"
#include <stdlib.h>

using namespace hls;
using namespace std;



int main(int argc, char *argv[])
{
	ifstream inputFile("../../../../input_trace.txt", ifstream::in);
	ofstream outputFile;
	outputFile.open("output.txt");
	if (!outputFile) {
		cerr << " Error opening output file!" << endl;
		return -1;
	}
		
	//initializing the QP context.
	QPInfo perQPInfo[N];

	for(int i = 0; i < N; i++) {
		perQPInfo[i].expectedSeq = 0;
		perQPInfo[i].curMSN = 0;
		perQPInfo[i].ooo_bitmap1 = 0;
		perQPInfo[i].ooo_bitmap2 = 0;
	}

	//reading number of lines in the input trace.
	int num_lines;
	inputFile >> num_lines;
	
	//reading each subsequent line.
	for (int i = 0; i < num_lines; i++) {
		int QID;
		ap_uint<5> opcode;
		ap_uint<24> seq;

		//reading metadata from input file.
		inputFile >> QID >> opcode >> seq;

		MetaData arrivedMetaData;
		arrivedMetaData.opcode = opcode;
		arrivedMetaData.seqNo = seq;
		stream<MetaData> arrivedMetaDataStream;
		arrivedMetaDataStream.write(arrivedMetaData);

		stream<AckInfo> newAckInfoStream;
		stream<QPInfo> perQPInfoStream, perQPInfoStreamOut;

		//write QP info to the input stream.
		perQPInfoStream.write(perQPInfo[QID]);

		receiveData(arrivedMetaDataStream, perQPInfoStream, perQPInfoStreamOut, newAckInfoStream);

		//read output streams.
		perQPInfo[QID] = perQPInfoStreamOut.read();
		AckInfo newAckInfo = newAckInfoStream.read();

		//write to output files.
		QPInfo tempQPInfo = perQPInfo[QID];
		outputFile << newAckInfo.ackSyndrome << " " << tempQPInfo.expectedSeq << " " << newAckInfo.sackNo << " " << tempQPInfo.curMSN << " " << newAckInfo.numCQEDone << "\n"; 
	}

	return 0;
}

