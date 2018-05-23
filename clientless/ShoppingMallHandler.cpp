#include "stdafx.h"

void CUser::ShoppingMall(Packet & pkt)
{
	uint8 opcode = pkt.read<uint8>();

	switch (opcode)
	{
	case STORE_OPEN:

		break;

	case STORE_CLOSE:
		HandleStoreClose(pkt);
		break;

	case STORE_BUY:
	case STORE_MINI: // not sure what this is
	case STORE_PROCESS:
		break;

	case STORE_LETTER:
		break;

	default:
		printf("Unknown shoppingmall packet: %X\n", opcode);
	}
}

void CUser::HandleStoreClose(Packet & pkt)
{
	for (int i = SLOT_MAX; i < SLOT_MAX+HAVE_MAX; i++)
	{
		pkt >> inv[i].itemID >> inv[i].sDurability >> inv[i].sCount >> inv[i].bFlag >> inv[i].sRentalTime;
	}
}
void CUser::StoreCloseRequest()
{
	Packet pkt(WIZ_LETTER, uint8(STORE_CLOSE));
	Send(&pkt);
}