/** incomplete Com code file used as a test to PDUR module */

#include "Com.h"

void ComTransmit(uint8_t Msg[]) {
	/** formulating I-PDU */
	const PduInfoType distanceMsg = {
		.SduDataPtr = Msg,
		.SduLength = sizeof(Msg) / sizeof(Msg[0])
	};
	/** query PDUR routing paths sources to find global pdu reference .
		inside COM module &Pdus[1] will be reference to related message .
	*/
	PduIdType PduHandleId;

	//TODO: simplify and use only one PDU Handle
	if (PduR_INF_GetSourcePduHandleId(&Pdus[1], &PduHandleId) == E_OK) {
		PduR_ComTransmit(PduHandleId, &distanceMsg);
		//PduR_ComCancelTransmit(PduHandleId);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PduR_ComTransmit_Test(void) {
	/** in case of COM module these lines exist in function that intend to transmit */
	/** formulating I-PDU */
	uint8_t distMsg[] = "ComTransmitMessage1234";
	const PduInfoType distanceMsg = {
		.SduDataPtr = distMsg,
		.SduLength = sizeof(distMsg) / sizeof(distMsg[0])
	};

	/** query PDUR routing paths sources to find global pdu reference .
		inside COM module &Pdus[1] will be reference to related message .
	*/
	PduIdType PduHandleId;

	if (PduR_INF_GetSourcePduHandleId(&Pdus[1], &PduHandleId) == E_OK) {
		PduR_ComTransmit(PduHandleId, &distanceMsg);
		PduR_ComCancelTransmit(PduHandleId);
	}
}

Std_ReturnType Com_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr) {
	/**
	the upper layer module (called module) shall check whether the
	available data fits into the buffer size reported by PduInfoPtr->SduLength.
	If it fits, it shall copy its data into the buffer provided by PduInfoPtr ->SduDataPtr
	and update the length of the actual copied data in PduInfoPtr->SduLength.
	If not, it returns E_NOT_OK without changing PduInfoPtr.
	*/
	printf("\nnow in Com Trigger Transmit\n");
	Std_ReturnType retVal = E_OK;
	uint8_t distMsg[] = "1234Msg";  //Message to be transmitted
	if (sizeof(distMsg) / sizeof(distMsg[0]) < PduInfoPtr->SduLength)
	{
		if (!memcpy((void *)PduInfoPtr->SduDataPtr,
			(void *)distMsg,
			sizeof(distMsg) / sizeof(distMsg[0]))
			) {
			retVal = E_NOT_OK;
		}
		else {
			PduInfoPtr->SduLength = sizeof(distMsg) / sizeof(distMsg[0]);
		}
	}
	else {
		retVal = E_NOT_OK;
	}
	return retVal;
}

void Com_RxIndication(PduIdType PduHandleId, const PduInfoType *PduInfoPtr) {
	printf("now in Com Rx Indication & message received is %s\n", PduInfoPtr->SduDataPtr);
	printf("Can -> Com reception indication has finished\n");
}

void Com_TxConfirmation(PduIdType PduHandleId, Std_ReturnType result) {
	printf("\nnow in Com TX confirmation\n");
	printf("Can -> Com confirmation has finished\n");
}

BufReq_ReturnType Com_StartOfReception(PduIdType id, const PduInfoType* info,
	PduLengthType TpSduLength, PduLengthType* bufferSizePtr) {

	printf("now in Com start of reception\n");
	return BUFREQ_OK;
}

BufReq_ReturnType Com_CopyRxData(PduIdType id, const PduInfoType* info,
	PduLengthType* bufferSizePtr) {

	printf("now in Com copy RX data\n");
	return BUFREQ_OK;
}

BufReq_ReturnType Com_CopyTxData(PduIdType id, const PduInfoType* info,
	RetryInfoType* retry, PduLengthType* availableDataPtr) {

	printf("now in Com copy TX data\n");
	return BUFREQ_OK;
}

/**
	Description  : query global I-PDU reference in Com configuration .
	inputs       : Pdu            | Reference to global PDU .
	output       : DestPduId      | Identifier to local I-PDU .
	I/O          : None
	return value : Std_ReturnType | Determine if I-PDU is exist or not.
*/
Std_ReturnType Com_INF_GetPduHandleId(Pdu_Type *Pdu, PduIdType *PduHandleIdPtr) {
	Std_ReturnType Std_Ret = E_NOT_OK;

	for (uint8_t i = 0; i < ComConfiguration.ComMaxIPduCnt; i++) {
		if (ComConfiguration.ComIPdu[i].PduIdRef == Pdu) {
			Std_Ret = E_OK;
			*PduHandleIdPtr = ComConfiguration.ComIPdu[i].IPduHandleId;
		}
	}
	return Std_Ret;
}