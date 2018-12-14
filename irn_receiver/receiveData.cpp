/* Author: Radhika Mittal
 * File description: receiveData module.
 */

#include "globals.hpp"

//switch-case based look up for pop-count of 8 bits 
//counting number of set bits in a bitmap of size 8.
inline ap_uint<4> popcount8(ap_uint<8> val) {
	switch(val) {
		case 0 : return 0;
		case 1 : 
		case 2 : 
		case 4 : 
		case 8 : 
		case 16 : 
		case 32 : 
		case 64 : 
		case 128 : return 1;
		case 3 : 
		case 5 : 
		case 6 : 
		case 9 : 
		case 10 : 
		case 12 : 
		case 17 : 
		case 18 : 
		case 20 : 
		case 24 : 
		case 33 : 
		case 34 : 
		case 36 : 
		case 40 : 
		case 48 : 
		case 65 : 
		case 66 : 
		case 68 : 
		case 72 : 
		case 80 : 
		case 96 : 
		case 129 : 
		case 130 : 
		case 132 : 
		case 136 : 
		case 144 : 
		case 160 : 
		case 192 : return 2;
		case 7 : 
		case 11 : 
		case 13 : 
		case 14 : 
		case 19 : 
		case 21 : 
		case 22 : 
		case 25 : 
		case 26 : 
		case 28 : 
		case 35 : 
		case 37 : 
		case 38 : 
		case 41 : 
		case 42 : 
		case 44 : 
		case 49 : 
		case 50 : 
		case 52 : 
		case 56 : 
		case 67 : 
		case 69 : 
		case 70 : 
		case 73 : 
		case 74 : 
		case 76 : 
		case 81 : 
		case 82 : 
		case 84 : 
		case 88 : 
		case 97 : 
		case 98 : 
		case 100 : 
		case 104 : 
		case 112 : 
		case 131 : 
		case 133 : 
		case 134 : 
		case 137 : 
		case 138 : 
		case 140 : 
		case 145 : 
		case 146 : 
		case 148 : 
		case 152 : 
		case 161 : 
		case 162 : 
		case 164 : 
		case 168 : 
		case 176 : 
		case 193 : 
		case 194 : 
		case 196 : 
		case 200 : 
		case 208 : 
		case 224 : return 3;
		case 15 : 
		case 23 : 
		case 27 : 
		case 29 : 
		case 30 : 
		case 39 : 
		case 43 : 
		case 45 : 
		case 46 : 
		case 51 : 
		case 53 : 
		case 54 : 
		case 57 : 
		case 58 : 
		case 60 : 
		case 71 : 
		case 75 : 
		case 77 : 
		case 78 : 
		case 83 : 
		case 85 : 
		case 86 : 
		case 89 : 
		case 90 : 
		case 92 : 
		case 99 : 
		case 101 : 
		case 102 : 
		case 105 : 
		case 106 : 
		case 108 : 
		case 113 : 
		case 114 : 
		case 116 : 
		case 120 : 
		case 135 : 
		case 139 : 
		case 141 : 
		case 142 : 
		case 147 : 
		case 149 : 
		case 150 : 
		case 153 : 
		case 154 : 
		case 156 : 
		case 163 : 
		case 165 : 
		case 166 : 
		case 169 : 
		case 170 : 
		case 172 : 
		case 177 : 
		case 178 : 
		case 180 : 
		case 184 : 
		case 195 : 
		case 197 : 
		case 198 : 
		case 201 : 
		case 202 : 
		case 204 : 
		case 209 : 
		case 210 : 
		case 212 : 
		case 216 : 
		case 225 : 
		case 226 : 
		case 228 : 
		case 232 : 
		case 240 : return 4;
		case 31 : 
		case 47 : 
		case 55 : 
		case 59 : 
		case 61 : 
		case 62 : 
		case 79 : 
		case 87 : 
		case 91 : 
		case 93 : 
		case 94 : 
		case 103 : 
		case 107 : 
		case 109 : 
		case 110 : 
		case 115 : 
		case 117 : 
		case 118 : 
		case 121 : 
		case 122 : 
		case 124 : 
		case 143 : 
		case 151 : 
		case 155 : 
		case 157 : 
		case 158 : 
		case 167 : 
		case 171 : 
		case 173 : 
		case 174 : 
		case 179 : 
		case 181 : 
		case 182 : 
		case 185 : 
		case 186 : 
		case 188 : 
		case 199 : 
		case 203 : 
		case 205 : 
		case 206 : 
		case 211 : 
		case 213 : 
		case 214 : 
		case 217 : 
		case 218 : 
		case 220 : 
		case 227 : 
		case 229 : 
		case 230 : 
		case 233 : 
		case 234 : 
		case 236 : 
		case 241 : 
		case 242 : 
		case 244 : 
		case 248 : return 5;
		case 63 : 
		case 95 : 
		case 111 : 
		case 119 : 
		case 123 : 
		case 125 : 
		case 126 : 
		case 159 : 
		case 175 : 
		case 183 : 
		case 187 : 
		case 189 : 
		case 190 : 
		case 207 : 
		case 215 : 
		case 219 : 
		case 221 : 
		case 222 : 
		case 231 : 
		case 235 : 
		case 237 : 
		case 238 : 
		case 243 : 
		case 245 : 
		case 246 : 
		case 249 : 
		case 250 : 
		case 252 : return 6;
		case 127 : 
		case 191 : 
		case 223 : 
		case 239 : 
		case 247 : 
		case 251 : 
		case 253 : 
		case 254 : return 7;
		case 255 : return 8;
	}
	return 0;
}

//popcount for 32 bits, invoking 8-bits popcount in parallel.
inline ap_uint<6> popcount32(ap_uint<32> val) {
	ap_uint<6> retVal = 0;
	ap_uint<8> valPart; 
	popCountLoop: for(int i=0; i < 4; i++) {
		#pragma HLS unroll
		valPart = (val & 0xff);
		retVal += popcount8(valPart);
		val = val >> 8;		
	}
	return retVal;
}



//the receiveData module.
void receiveData(stream<MetaData> &arrivedMetaData, stream<QPInfo> &perQPInfoIn, stream<QPInfo> &perQPInfoOut, stream<AckInfo> &newAckInfo) {
	//#pragma HLS pipeline
	#pragma HLS STREAM variable=arrivedMetaData 
	#pragma HLS STREAM variable=newAckInfo 
	#pragma HLS STREAM variable=perQPInfoIn 
	#pragma HLS STREAM variable=perQPInfoOut


	MetaData localArrivedMetaData = arrivedMetaData.read();
	QPInfo localPerQPInfo = perQPInfoIn.read();

	ap_uint<LOGBDP> localNumCQEDone = 0;
	bool localAckSyndrome = 0;
	
	//checking if the packet will generate an MSN update or a CQE generation.
	bool lastOrOnly = false;
	bool cqeNeeded = false;
	if((localArrivedMetaData.opcode == SEND_LAST) || (localArrivedMetaData.opcode == SEND_LAST_IMM) || (localArrivedMetaData.opcode == SEND_ONLY) || (localArrivedMetaData.opcode == SEND_ONLY_IMM)) {
		lastOrOnly = true;
		cqeNeeded = true;
	} else {
		if((localArrivedMetaData.opcode == WRITE_LAST) || (localArrivedMetaData.opcode == WRITE_ONLY) || (localArrivedMetaData.opcode == READ_REQ)) 
			lastOrOnly = true;
		else if((localArrivedMetaData.opcode == WRITE_LAST_IMM) || (localArrivedMetaData.opcode == WRITE_ONLY_IMM)) {
			lastOrOnly = true;
			cqeNeeded = true;
		}
	}

	//if the packet corresponds to expected sequence number,
	//advance the bitmaps and increase expected sequence, message sequence (MSN),
	//and number of CQEs completed accordingly. 
	expectedPktReceived: if(localArrivedMetaData.seqNo == localPerQPInfo.expectedSeq) {
		localPerQPInfo.expectedSeq++;
		if(lastOrOnly) localPerQPInfo.curMSN++;
		if(cqeNeeded) localNumCQEDone++;
		ap_uint<BDP> temp = localPerQPInfo.ooo_bitmap1 | localPerQPInfo.ooo_bitmap2;
		bitmapNotEmpty: if(temp != 0) {
			localPerQPInfo.ooo_bitmap1 = localPerQPInfo.ooo_bitmap1 >> 1;
			localPerQPInfo.ooo_bitmap2 = localPerQPInfo.ooo_bitmap2 >> 1;

			ap_uint<LOGBDP> msnInc = 0, bits_to_shift = 0;

			ap_uint<LOGBDP> bits_to_shiftArr[BDPBY32], msnIncArr[BDPBY32], localNumCQEDoneArr[BDPBY32];

			collectPartStats: for(int i=0; i < BDPBY32; i++) {
			#pragma HLS unroll
				int idx = i << 5;
				ap_uint<32> part1 = localPerQPInfo.ooo_bitmap1.range(idx + 31, idx);;
				ap_uint<32> part2 = localPerQPInfo.ooo_bitmap2.range(idx + 31, idx);;
				ap_uint<32> part = (part1 | part2);
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
				part2 = part2 << (32 - bits_to_shift_part);
				part2 = part2 >> (32 - bits_to_shift_part);
				msnIncArr[i] = popcount32(part2);
				localNumCQEDoneArr[i] = popcount32(part1 & part2);
				bits_to_shiftArr[i] = bits_to_shift_part;

				bool factor = 1;
				if(i > 0)
					factor = bool(ap_uint<LOGBDP>(bits_to_shiftArr[i - 1] >> 5));

				bits_to_shiftArr[i] *= factor;
				msnIncArr[i] *= factor;
				localNumCQEDoneArr[i] *= factor;

				bits_to_shift += bits_to_shiftArr[i];
				msnInc += msnIncArr[i];
				localNumCQEDone += localNumCQEDoneArr[i];
				
			}

			localPerQPInfo.ooo_bitmap1 = localPerQPInfo.ooo_bitmap1 >> bits_to_shift;
			localPerQPInfo.ooo_bitmap2 = localPerQPInfo.ooo_bitmap2 >> bits_to_shift;
			localPerQPInfo.expectedSeq += bits_to_shift;
			localPerQPInfo.curMSN += msnInc;
		}
	//if arrived packet's sequence number is greater than expected, prepare a NACK and mark bitmap.	 
	} else if(localArrivedMetaData.seqNo > localPerQPInfo.expectedSeq) {
		localAckSyndrome = 1;
		ap_uint<BDP> temp = 1;
		temp = temp << (localArrivedMetaData.seqNo - localPerQPInfo.expectedSeq);
		if(lastOrOnly) {
			localPerQPInfo.ooo_bitmap2 = localPerQPInfo.ooo_bitmap2 | temp;	
		}
		if(cqeNeeded || (!lastOrOnly)) {
			localPerQPInfo.ooo_bitmap1 = localPerQPInfo.ooo_bitmap1 | temp;	
		}
	}
		

	//write output.
	AckInfo localNewAckInfo;
	localNewAckInfo.ackSyndrome = localAckSyndrome;
	localNewAckInfo.cumAck = localPerQPInfo.expectedSeq;
	localNewAckInfo.curMSN = localPerQPInfo.curMSN;
	localNewAckInfo.sackNo = localArrivedMetaData.seqNo;
	localNewAckInfo.numCQEDone = localNumCQEDone;
	newAckInfo.write(localNewAckInfo);

	perQPInfoOut.write(localPerQPInfo);
}

