#include <stdio.h>
#include <stdlib.h>

#include "Com.h"
#include "PduR.h"

int main() {
	/*
	Initialize all the modules
	*/
	PduR_Init(&PBPduRConfig);
	//CanIf_Init(&CanIf_Config);

	ComSendMessage("LMFAO");

	return 0;
}
