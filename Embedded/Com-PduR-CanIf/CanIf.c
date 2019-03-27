
#include "CanIf.h"
#include "CanIf_Cbk.h"
//file to be created by PDUR and Used Here
#include "PduR_CanIf.h"
#include "stdio.h"

//---------------------------------------------------------------------------

CanIf_ControllerModeType currentControllerMode;
const CanIf_ConfigType *canIf_ConfigPtr;

//-----------------------------------------------------------------------------------------

/*
in: ConfigPtr Pointer to configuration parameter set, used e.g. for post build parameters
This service Initializes internal and external interfaces of the CAN Interface for the further processing.
 */

void CanIf_Init(const CanIf_ConfigType *ConfigPtr) {
	/** When function CanIf_Init() is called, CanIf shall initialize
		every Transmit L-PDU Buffer assigned to CanIf.
	 */
	if (ConfigPtr != 0) {
		canIf_ConfigPtr = ConfigPtr;
		currentControllerMode = CANIF_CS_UNINIT;
	}
	else {
		printf("CanIf_Init fn -> ConfigPtr = NULL");
	}

	// for(uint8 i=0; i<CANIF_INF_CAN_DRIVER_0_CONTROLER_CNT; i++){
	//     CanIf_SetPduMode(i, CANIF_OFFLINE);
	// }
}

Std_ReturnType CanIf_GetControllerMode(uint8 ControllerId, CanIf_ControllerModeType *ControllerModePtr) {
	if (canIf_ConfigPtr == 0 || ControllerModePtr == 0) {
		return E_NOT_OK;
	}
	ControllerModePtr = &currentControllerMode;
	return E_OK;
}
//-----------------------------------------------------------------------------
//initiates a transition to the requested CAN controller mode ControllerMode of the CAN controller
// which is assigned by parameter ControllerId.
/*
Std_ReturnType CanIf_SetControllerMode(uint8 ControllerId, CanIf_ControllerModeType ControllerMode) {
	if (ControllerMode == currentControllerMode) {
		printf("Already in This State");
		return E_OK;
	}
	switch (ControllerMode) {
		case CANIF_CS_SLEEP:
			if (currentControllerMode == CANIF_CS_STOPPED) {
				if (Can_SetControllerMode(ControllerId, CAN_T_SLEEP) == CAN_OK) {
					currentControllerMode = CANIF_CS_SLEEP;
					printf("current Controller mode is CANIF_CS_SLEEP");
					return E_OK;
				} else {
					return E_NOT_OK;
				}
			}
			break;
		case CANIF_CS_STARTED:
			if (currentControllerMode == CANIF_CS_STOPPED) {
				if (Can_SetControllerMode(ControllerId, CAN_T_START) == CAN_OK) {
					currentControllerMode = CANIF_CS_STARTED;
					printf("current Controller mode is CANIF_CS_STARTED");
					return E_OK;
				} else
					return E_NOT_OK;
			}
			break;
		case CANIF_CS_STOPPED:
			if (currentControllerMode == CANIF_CS_STARTED) {
				if (Can_SetControllerMode(ControllerId, CAN_T_STOP) == CAN_OK) {
					currentControllerMode = CANIF_CS_STOPPED;
					printf("current Controller mode is CANIF_CS_STOPPED");
					return E_OK;
				} else
					return E_NOT_OK;
			} else if (currentControllerMode == CANIF_CS_SLEEP) {
				if (Can_SetControllerMode(ControllerId, CAN_T_WAKEUP) == CAN_OK) {
					currentControllerMode = CANIF_CS_STOPPED;
					printf("current Controller mode is CANIF_CS_STOPPED");
					return E_OK;
				} else
					return E_NOT_OK;
			}
			break;
		case CANIF_CS_UNINIT:
			printf("CanIf_SetControllerMode : CANIF_CS_UNINIT case");
			break;
			break;
		default:
			printf("CanIf_SetControllerMode : default case");
			break;
	}
	return E_OK;
}
 */
 //-----------------------------------------------------------------------------

/*
CanIfTxSduId L-SDU handle to be transmitted.This handle specifies the corresponding CAN L-SDU ID and
implicitly the CAN Driver instance as well as the corresponding CAN controller device.
CanIfTxInfoPtr Pointer to a structure with CAN L-SDU related data: DLC and pointer to
CAN L-SDU buffer including the MetaData of dynamic L-PDUs.
This service initiates a request for transmission of the CAN L-PDU specified by the CanTxSduId and CAN related
data in the L-SDU structure.
 */

Std_ReturnType CanIf_Transmit(PduIdType CanIfTxSduId, const PduInfoType *CanIfTxInfoPtr) {
	if (canIf_ConfigPtr == 0 || CanIfTxInfoPtr == 0) {
		printf("CanIf_Transmit : CanIF is not initialized or no Data sent");
		return E_NOT_OK;
	}
	const CanIfTxPduCfg *txEntry = (CanIfTxPduCfg *)(&canIf_ConfigPtr->canIfInitCfg->canIfTxPduCfg[CanIfTxSduId]);

	//prepare the PDU data
	Can_PduType canPdu;
	canPdu.id = txEntry->canIfTxPduCanId;
	canPdu.length = CanIfTxInfoPtr->SduLength;
	canPdu.sdu = CanIfTxInfoPtr->SduDataPtr;
	canPdu.swPduHandle = CanIfTxSduId;


	//TTTT
	/*Can_ReturnType retVal = Can_Write(txEntry->canIfTxPduBufferRef->canIfBufferHthRef->canIfHthIdSymRef->canObjectId, &canPdu);
		if (retVal == CAN_NOT_OK || retVal == CAN_BUSY) {
			return E_NOT_OK;
		}*/


	return E_OK;

}

/*
in : CanTxPduId L-PDU handle of CAN L-PDU successfully transmitted.This ID specifies the corresponding CAN L-PDU ID
				and implicitly the CAN Driver instance as well as the corresponding CAN controller device.
This service confirms a previously successfully processed transmission of a CAN TxPDU.
 */

 //for now set CanTxPduID to zero
void CanIf_TxConfirmation(PduIdType CanTxPduId) {
	if (canIf_ConfigPtr == 0) {
		printf("CanIf_TxConfirmation : The CanIf is not initialized");
		return;
	}
	if (CanTxPduId > canIf_ConfigPtr->canIfInitCfg->canIfMaxTxPduCfg) {
		printf("CanIf_TxConfirmation : exceeded THE MAX Number of IDs");
		return;
	}
	CanIfTxPduCfg *TxPduCfgPtr = (CanIfTxPduCfg *)(&canIf_ConfigPtr->canIfInitCfg->canIfTxPduCfg[CanTxPduId]);

	//    if (TxPduCfgPtr == null) {
	//        printf("CanIf_TxConfirmation : TxPduCfgPtr = NULL ");
	//        return;
	//    }

	(TxPduCfgPtr->canIfTxPduUserTxConfirmationName)(CanTxPduId);
}

/*
in : Mailbox Identifies the HRH and its corresponding CAN Controller
	 PduInfoPtr Pointer to the received L-PDU
This service indicates a successful reception of a received CAN Rx L-PDU to the CanIf after passing all filters and validation checks.
 */

static CanIfRxPduCfg *findRxPduCfg(Can_HwHandleType Hoh) {
	for (uint32 i = 0; i != canIf_ConfigPtr->canIfInitCfg->canIfMaxRxPduCfg; ++i) {
		if (Hoh == (canIf_ConfigPtr)->canIfInitCfg->canIfRxPduCfg[i].canIfRxPduHrhIdRef->canIfHrhIdSymRef->canObjectId) {
			return (CanIfRxPduCfg * const)(&canIf_ConfigPtr->canIfInitCfg->canIfRxPduCfg[i]);
		}
	}
	return 0;
}

void CanIf_RxIndication(const Can_HwType *Mailbox, const PduInfoType *PduInfoPtr) {
	const CanIfRxPduCfg *RxPduCfgPtr;
	CanIf_PduModeType PduMode;
	PduIdType RxPduId;

	if (Mailbox == 0 || PduInfoPtr == 0) {
		///Report an error CANIF_E_PARAM_POINTER to the Det_ReportError
		return;
	}

	RxPduCfgPtr = findRxPduCfg(Mailbox->Hoh);
	//
	if (RxPduCfgPtr == 0) {
		///report development error code CANIF_E_PARAM_HOH to the Det
		return;
	}

	if (PduInfoPtr->SduLength != RxPduCfgPtr->canIfRxPduDlc) {
		///report development error code CANIF_E_INVALID_DATA_LENGTH to the Det
		return;
	}

	(RxPduCfgPtr->canIfRxPduUserRxIndicationName)(RxPduCfgPtr->CanIfRxPduId, PduInfoPtr);
}