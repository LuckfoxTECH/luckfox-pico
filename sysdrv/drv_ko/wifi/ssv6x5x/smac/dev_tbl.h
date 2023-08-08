/*
 * Copyright (c) 2015 iComm-semi Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _DEV_TBL_H_
#define _DEV_TBL_H_

#include "drv_comm.h"
#include <ssv6200_common.h>

//action
#define ACTION_DO_NOTHING	0	//00
#define ACTION_UPDATE_NAV	1	//01
#define ACTION_RESET_NAV	2	//10
#define ACTION_SIGNAL_ACK	3	//11
		
			//frame
#define FRAME_ACCEPT			0
#define FRAME_DROP				1
		
			
#define SET_DEC_TBL( _type, _mask, _action, _drop) 		\
				(_type<<9|			\
				_mask <<3|			\
				_action<<1| 		\
				_drop)
				
#ifndef USE_GENERIC_DECI_TBL
	/**
	* SSV6200 MAC Decision Table for STA mode. The first and 
	* second tables are aggregated into a single table here.
	*/
	u16 sta_deci_tbl[] =
	{
		//---------------------------------------------------------------------------------------------------------------------------
		/* The first decision table content: */
		//---------------------------------------------------------------------------------------------------------------------------
		SET_DEC_TBL(0x1e, 0x3e, ACTION_RESET_NAV, FRAME_DROP), 		//0x3DF5, /* 0 011110 111110 10 1 */     //CF-End (1110) +   CF-End + CF-Ack (1111) --> drop
		SET_DEC_TBL(0x18, 0x3e, ACTION_SIGNAL_ACK, FRAME_ACCEPT),	//0x31F6, /* 0 011000 111110 11 0 */     //BlockAckReq (1000) + BlockAck (1001)
		SET_DEC_TBL(0x1a, 0x3f, ACTION_SIGNAL_ACK, FRAME_DROP),		//0x35F9, /* 0 011010 111111 00 1 */     //PS-Poll (1010) --> drop
		SET_DEC_TBL(0x10, 0x38, ACTION_DO_NOTHING, FRAME_DROP), 	//0x21C1, /* 0 010000 111000 00 1 */	   //Control Wrapper (0111) --> drop

		SET_DEC_TBL(0x25, 0x3f, ACTION_DO_NOTHING, FRAME_DROP),		//0x4BF9, /* 0 100101 111111 00 1 */     //
		SET_DEC_TBL(0x26, 0x36, ACTION_DO_NOTHING, FRAME_DROP),		//0x4DB1, /* 0 100110 110110 00 1 */
		SET_DEC_TBL(0x08, 0x3f, ACTION_DO_NOTHING, FRAME_ACCEPT),	//0x11F8  /* 0 001000 111111 00 0 */	   //Beacon
		SET_DEC_TBL(0x05, 0x3f, ACTION_SIGNAL_ACK, FRAME_ACCEPT),		//0x0BFE, /* 0 000101 111111 11 0 */     //Probe Response

		SET_DEC_TBL(0x0b, 0x3f, ACTION_SIGNAL_ACK, FRAME_ACCEPT),		//0x17FE, /* 0 001011 111111 11 0 */     //Auth
		SET_DEC_TBL(0x01, 0x3d, ACTION_SIGNAL_ACK, FRAME_ACCEPT),		//0x03EE, /* 0 000001 111101 11 0 */     //Assoc/ReAssoc
		SET_DEC_TBL(0x20, 0x30, ACTION_SIGNAL_ACK, FRAME_ACCEPT),		//0x4186, /* 0 100000 110000 11 0 */     //Data
		SET_DEC_TBL(0x00, 0x00, ACTION_SIGNAL_ACK, FRAME_ACCEPT),		//0x0006, /* 0 000000 000000 11 0 */

		SET_DEC_TBL(0x00, 0x00, ACTION_DO_NOTHING, FRAME_DROP),		//0x0001, /* 0 000000 000000 00 1 */
		SET_DEC_TBL(0x00, 0x00, ACTION_UPDATE_NAV, FRAME_DROP),		//0x0003, /* 0 000000 000000 01 1 */     //CTS for self
		SET_DEC_TBL(0x00, 0x00, ACTION_RESET_NAV, FRAME_DROP),		//0x0005, /* 0 000000 000000 10 1 */
		SET_DEC_TBL(0x00, 0x00, ACTION_SIGNAL_ACK, FRAME_DROP),		//0x0007, /* 0 000000 000000 11 1 */     //??? RTS (1011) + CTS (1100 ) + ACK (1101) --> drop

		//---------------------------------------------------------------------------------------------------------------------------
		/* The second decision table contents: decision mask */
		//---------------------------------------------------------------------------------------------------------------------------
		0x2008,
		0x1001,
		//    0x8408, //0x0808,  <-- for dhcp offer data ( DA --> broadcast )
		//    0x1000,//when user request to scan, it should set to 0x1040.
#if 0    
		0x8408, //0x0808,  <-- for dhcp offer data ( DA --> broadcast )
		0x1000, //when user request to scan, it should set to 0x1040.
#else
		0x0808, //0x0808,  <-- for dhcp offer data ( DA --> broadcast )
		0x1040, //when user request to scan, it should set to 0x1040.
#endif

		0x2008,
		0x800E,
		0x0BB8,//unicat data/mgmt to me in the same bssid or wildcardbssid ( Auth/Assoc/ )
		0x2B88,//unicat data/mgmt to me in different bssid ( Probe Response/Auth )
		0x0800,
	};



	/**
	* SSV6200 MAC Decision Table for AP mode:
	*/
	u16 ap_deci_tbl[] =
	{
		//---------------------------------------------------------------------------------------------------------------------------
		/* The first decision table content: */
		//---------------------------------------------------------------------------------------------------------------------------
		SET_DEC_TBL(0x1e, 0x3e, ACTION_RESET_NAV, FRAME_DROP), 		//0x3DF5, /* 0 011110 111110 10 1 */     //CF-End (1110) +   CF-End + CF-Ack (1111) --> drop					//Reset NAV			//Drop
		SET_DEC_TBL(0x18, 0x3e, ACTION_SIGNAL_ACK, FRAME_ACCEPT), 	//0x31F6, /* 0 011000 111110 11 0 */     //BlockAckReq (1000) + BlockAck (1001)								//Signal(Ack)			//Accept
		SET_DEC_TBL(0x1a, 0x3f, ACTION_SIGNAL_ACK, FRAME_ACCEPT), //0x35F8, /* 0 011010 111111 11 0 */     //PS-Poll (1010) --> accept										//Do nothing			//Accept
		SET_DEC_TBL(0x10, 0x38, ACTION_DO_NOTHING, FRAME_DROP), //0x21C1, /* 0 010000 111000 00 1 */	   	//Control Wrapper (0111) --> drop									//Do nothing			//Drop
		//-4
		SET_DEC_TBL(0x25, 0x3f, ACTION_DO_NOTHING, FRAME_DROP), //0x4BF9, /* 0 100101 111111 00 1 */     //([Data]CF-ACK)												//Do nothing			//Drop
		SET_DEC_TBL(0x26, 0x36, ACTION_DO_NOTHING, FRAME_DROP), //0x4DB1, /* 0 100110 110110 00 1 */		//([Data]all CF-Poll (no data)	  )									//Do nothing			//Drop
		SET_DEC_TBL(0x08, 0x3f, ACTION_DO_NOTHING, FRAME_DROP), //0x11F9, /* 0 001000 111111 00 1 */	   	//Beacon			---->drop										//Do nothing			//Drop
		SET_DEC_TBL(0x20, 0x30, ACTION_DO_NOTHING, FRAME_DROP), //0x4181, /* 0 100000 110000 00 1 */    	//Drop all mc/broadcast data frames.(AP don't need to receive bc/multi)		//Do nothing			//Drop
		//-8
		SET_DEC_TBL(0x00, 0x00, ACTION_DO_NOTHING, FRAME_ACCEPT), //0x0000, /* 0 000000 000000 00 0 */     //Empty														//Do nothing			//Accept
		SET_DEC_TBL(0x00, 0x00, ACTION_DO_NOTHING, FRAME_ACCEPT), //0x0000, /* 0 000000 000000 00 0 */     //Empty														//Do nothing			//Accept
		SET_DEC_TBL(0x20, 0x30, ACTION_SIGNAL_ACK, FRAME_ACCEPT), //0x4186, /* 0 100000 110000 11 0 */     //Data														//Signal(Ack)			//Accept
		SET_DEC_TBL(0x00, 0x00, ACTION_SIGNAL_ACK, FRAME_ACCEPT), //0x0006, /* 0 000000 000000 11 0 */		//All frames													//Signal(Ack)			//Accept
		//-12
		SET_DEC_TBL(0x00, 0x00, ACTION_DO_NOTHING, FRAME_DROP), //0x0001, /* 0 000000 000000 00 1 */		//All frames													//Do nothing			//Drop
		SET_DEC_TBL(0x00, 0x00, ACTION_UPDATE_NAV, FRAME_DROP), //0x0003, /* 0 000000 000000 01 1 */     //CTS for self													//Update NAV			//Drop
		SET_DEC_TBL(0x00, 0x00, ACTION_RESET_NAV, FRAME_DROP), //0x0005, /* 0 000000 000000 10 1 */		//All frames													//Reset NAV			//Drop
		SET_DEC_TBL(0x00, 0x00, ACTION_SIGNAL_ACK, FRAME_DROP), //0x0007, /* 0 000000 000000 11 1 */     //??? RTS (1011) + CTS (1100 ) + ACK (1101) --> drop					//Signal(Ack)			//Drop


		//---------------------------------------------------------------------------------------------------------------------------
		/* The second decision table contents: decision mask */
		//---------------------------------------------------------------------------------------------------------------------------

		0x2008, //ToDS, FromDS error with AP mode/station mode
		0x1001, //Control frames with BC/MC RA. CF-End, CF-End+CF-Ack
		0x0888, //Mcast or Bcast data/mgmt frames with same or wildcard BSSID.           0x0808,  <-- for dhcp offer data ( DA --> broadcast )
		0x1040, //Mcast data/mgmt frames with different BSSID.                                 when user request to scan, it should set to 0x1040.

		0x2008,	//Unicast frames not to me
		0x800E, //Unicast Control Frames to me
		//0x0838, //Unicat data/mgmt to me in the same bssid or wildcardbssid 

		0x0800,
		0x2008, //0x2808, //Unicat data/mgmt to me in different bssid 
		0x0800, //WDS
	};

#else // USE_GENERIC_DECI_TBL
u16 generic_deci_tbl[] =
    {
            //---------------------------------------------------------------------------------------------------------------------------
            /* The first decision table content: */
            //---------------------------------------------------------------------------------------------------------------------------
    /* 0 */ SET_DEC_TBL(0x1e, 0x3e, ACTION_RESET_NAV,  FRAME_DROP),    //0x3DF5, /* 0 011110 111110 10 1 */    //CF-End (1110) + CF-End + CF-Ack (1111) --> drop               //Reset NAV
    /* 1 */ SET_DEC_TBL(0x18, 0x3e, ACTION_SIGNAL_ACK, FRAME_ACCEPT),  //0x31F6, /* 0 011000 111110 11 0 */    //BlockAckReq (1000) + BlockAck (1001)                                  //Signal(Ack)
    /* 2 */ SET_DEC_TBL(0x1a, 0x3f, ACTION_DO_NOTHING, FRAME_ACCEPT),  //0x35F8, /* 0 011010 111111 00 0 */    //PS-Poll (1010) --> accept                                                         //Do nothing
    /* 3 */ SET_DEC_TBL(0x10, 0x38, ACTION_DO_NOTHING, FRAME_DROP),    //0x21C1, /* 0 010000 111000 00 1 */    //Control Wrapper (0111) --> drop

    /* 4 */ SET_DEC_TBL(0x08, 0x3F, ACTION_DO_NOTHING, FRAME_DROP),    //0x11F9, /* 0 001000 111111 00 1 */    //Beacon (1000) --> drop
    /* 5 */ SET_DEC_TBL(0x04, 0x3F, ACTION_DO_NOTHING, FRAME_DROP),    //0x09F9, /* 0 000100 111111 00 1 */    //Probe Request (0100) --> drop
    /* 6 */ 0,
    /* 7 */ SET_DEC_TBL(0x05, 0x3f, ACTION_SIGNAL_ACK, FRAME_ACCEPT),   //0x0BFE, /* 0 000101 111111 11 0 */     //Probe Response

    /* 8 */ SET_DEC_TBL(0x0b, 0x3f, ACTION_SIGNAL_ACK, FRAME_ACCEPT),   //0x17FE, /* 0 001011 111111 11 0 */     //Auth
    /* 9 */ SET_DEC_TBL(0x01, 0x3d, ACTION_SIGNAL_ACK, FRAME_ACCEPT),   //0x03EE, /* 0 000001 111101 11 0 */     //Assoc/ReAssoc
    /* a */ SET_DEC_TBL(0x00, 0x00, ACTION_DO_NOTHING, FRAME_ACCEPT),   //0x0006, /* 0 000000 000000 11 0 */    //All frames
    /* b */ SET_DEC_TBL(0x00, 0x00, ACTION_SIGNAL_ACK, FRAME_ACCEPT),   //0x0006, /* 0 000000 000000 11 0 */    //All frames

    /* c */ SET_DEC_TBL(0x00, 0x00, ACTION_DO_NOTHING, FRAME_DROP),     //0x0001, /* 0 000000 000000 00 1 */    //All frames
    /* d */ SET_DEC_TBL(0x00, 0x00, ACTION_UPDATE_NAV, FRAME_DROP),     //0x0003, /* 0 000000 000000 01 1 */    //CTS for self
    /* e */ SET_DEC_TBL(0x00, 0x00, ACTION_RESET_NAV,  FRAME_DROP),     //0x0005, /* 0 000000 000000 10 1 */    //All frames
    /* f */ SET_DEC_TBL(0x00, 0x00, ACTION_SIGNAL_ACK, FRAME_DROP),     //0x0007, /* 0 000000 000000 11 1 */    //??? RTS (1011) + CTS (1100 ) + ACK (1101) --> drop    //Signal(Ack)           //Drop

            //---------------------------------------------------------------------------------------------------------------------------
            /* The second decision table contents: decision mask */
            //---------------------------------------------------------------------------------------------------------------------------
            //             fedc ba98 7654 3210
            //           -----------------------
            0x2008, // 0 - 0010 0000 0000 1000 - ToDS, FromDS error with AP mode/station mode
            0x1001, // 1 - 0001 0000 0000 0001 - Control frames with BC/MC RA. CF-End, CF-End+CF-Ack
            0x0400, // 2 - 0000 0100 0000 0000 - Mcast or Bcast data/mgmt frames with same or wildcard BSSID.
            0x0400, // 3 - 0000 0100 0000 0000 - Mcast or Bcast data/mgmt frames with different BSSID. To support dual interface, accept as BSSID matched.
            0x2000, // 4 - 0010 0000 0000 0000 - Unicast frames not to me
            0x800E, // 5 - 1000 0000 0000 1110 - Unicast Control Frames to me
            0x0800, // 6 - 0000 1000 0000 0000 - Unicast frames to me
            0x0B88, // 7 - 0000 1000 0000 0000 - Unicat data/mgmt to me in different bssid. To support dual interface, accept as BSSID matched.
            0x0800, // 8 - 0000 1000 0000 0000 - WDS
    };
#endif // USE_GENERIC_DECI_TBL

#ifdef USE_CONCURRENT_DECI_TBL
u16 concurrent_deci_tbl[] =
    {
            //---------------------------------------------------------------------------------------------------------------------------
            /* The first decision table content: */
            //---------------------------------------------------------------------------------------------------------------------------
    /* 0 */ SET_DEC_TBL(0x1e, 0x3e, ACTION_RESET_NAV,  FRAME_DROP),    //0x3DF5, /* 0 011110 111110 10 1 */     //CF-End (1110) + CF-End + CF-Ack (1111) --> drop               //Reset NAV
    /* 1 */ SET_DEC_TBL(0x18, 0x3e, ACTION_SIGNAL_ACK, FRAME_ACCEPT),  //0x31F6, /* 0 011000 111110 11 0 */     //BlockAckReq (1000) + BlockAck (1001)                                  //Signal(Ack)
    /* 2 */ SET_DEC_TBL(0x1a, 0x3f, ACTION_DO_NOTHING, FRAME_ACCEPT),  //0x35F8, /* 0 011010 111111 00 0 */     //PS-Poll (1010) --> accept                                                         //Do nothing
    /* 3 */ SET_DEC_TBL(0x10, 0x38, ACTION_DO_NOTHING, FRAME_DROP),    //0x21C1, /* 0 010000 111000 00 1 */     //Control Wrapper (0111) --> drop

    /* 4 */ SET_DEC_TBL(0x08, 0x3f, ACTION_DO_NOTHING, FRAME_ACCEPT),   //0x11F8  /* 0 001000 111111 00 0 */    //Beacon
    /* 5 */ 0,
    /* 6 */ 0,
    /* 7 */ SET_DEC_TBL(0x05, 0x3f, ACTION_SIGNAL_ACK, FRAME_ACCEPT),   //0x0BFE, /* 0 000101 111111 11 0 */    //Probe Response

    /* 8 */ SET_DEC_TBL(0x0b, 0x3f, ACTION_SIGNAL_ACK, FRAME_ACCEPT),   //0x17FE, /* 0 001011 111111 11 0 */    //Auth
    /* 9 */ SET_DEC_TBL(0x01, 0x3d, ACTION_SIGNAL_ACK, FRAME_ACCEPT),   //0x03EE, /* 0 000001 111101 11 0 */    //Assoc/ReAssoc
    /* a */ SET_DEC_TBL(0x00, 0x00, ACTION_DO_NOTHING, FRAME_ACCEPT),   //0x0006, /* 0 000000 000000 11 0 */    //All frames
    /* b */ SET_DEC_TBL(0x00, 0x00, ACTION_SIGNAL_ACK, FRAME_ACCEPT),   //0x0006, /* 0 000000 000000 11 0 */    //All frames

    /* c */ SET_DEC_TBL(0x00, 0x00, ACTION_DO_NOTHING, FRAME_DROP),     //0x0001, /* 0 000000 000000 00 1 */    //All frames
    /* d */ SET_DEC_TBL(0x00, 0x00, ACTION_UPDATE_NAV, FRAME_DROP),     //0x0003, /* 0 000000 000000 01 1 */    //CTS for self
    /* e */ SET_DEC_TBL(0x00, 0x00, ACTION_RESET_NAV,  FRAME_DROP),     //0x0005, /* 0 000000 000000 10 1 */    //All frames
    /* f */ SET_DEC_TBL(0x00, 0x00, ACTION_SIGNAL_ACK, FRAME_DROP),     //0x0007, /* 0 000000 000000 11 1 */    //??? RTS (1011) + CTS (1100 ) + ACK (1101) --> drop    //Signal(Ack)           //Drop

            //---------------------------------------------------------------------------------------------------------------------------
            /* The second decision table contents: decision mask */
            //---------------------------------------------------------------------------------------------------------------------------
            //             fedc ba98 7654 3210
            //           -----------------------
            0x2008, // 0 - 0010 0000 0000 1000 - ToDS, FromDS error with AP mode/station mode
            0x1001, // 1 - 0001 0000 0000 0001 - Control frames with BC/MC RA. CF-End, CF-End+CF-Ack
            0x0400, // 2 - 0000 0100 0000 0000 - Mcast or Bcast data/mgmt frames with same or wildcard BSSID.
            0x1010, // 3 - 0001 0000 0000 0000 - Mcast or Bcast data/mgmt frames with different BSSID. Drop mismatch frame.
            0x2000, // 4 - 0010 0000 0000 0000 - Unicast frames not to me
            0x800E, // 5 - 1000 0000 0000 1110 - Unicast Control Frames to me
            0x0800, // 6 - 0000 1000 0000 0000 - Unicast frames to me
            0x1000, // 7 - 0001 0000 0000 0000 - Unicat data/mgmt to me in different bssid. Drop mismatch frame.
            0x0800, // 8 - 0000 1000 0000 0000 - WDS
            0x0400, // 9 - 0000 0100 0000 0000 - Mcast or Bcast data/mgmt frames with same or wildcard BSSID1.
            0x0800, // 10 - 0000 1000 0000 0000 - Unicast frames to me and BSSID = BSSID1.
    };
#endif



#define SET_PHY_INFO( _ctsdur, _ba_rate_idx, _ack_rate_idx, _llength_idx, _llength_enable) \
									(_ctsdur<<16|			\
									_ba_rate_idx <<10|		\
									_ack_rate_idx<<4|		\
									_llength_idx<<1|		\
									_llength_enable)

//#define UPDATE_PHY_INFO_ACK_RATE(_phy_info, _ack_rate_idx) ( _phy_info = (_phy_info&0xfffffc0f)|(_ack_rate_idx<<4))



#define SET_PHY_L_LENGTH( _l_ba, _l_rts, _l_cts_ack ) 		 (_l_ba<<12|_l_rts<<6 |_l_cts_ack)


/**
* SSV6200 PHY Info Table for both AP & STA mode.
*
*/
u32 phy_info_tbl[] =
	{
		/* PHY Infor Table: */
		0x0C000000, 0x0C000100, 0x0C000200, 0x0C000300, 0x0C000140, 
		0x0C000240, 0x0C000340, 0x00000001, 0x00000101, 0x00000201, 
		0x00000301, 0x0C000401, 0x0C000501, 0x0C000601, 0x0C000701, 
		0x00030002, 0x00030102, 0x00030202, 0x0C030302, 0x0C030402, 
		0x0C030502, 0x0C030602, 0x10030702, 0x00030082, 0x00030182, 
		0x00030282, 0x0C030382, 0x0C030482, 0x0C030582, 0x0C030682, 
		0x10030782, 0x00030042, 0x00030142, 0x00030242, 0x0C030342, 
		0x0C030442, 0x0C030542, 0x0C030642, 0x10030742,


		/* PHY Index Table: */

		//B	
		SET_PHY_INFO(314, 0, 0, 0, 0),	//0x013A0000,
		SET_PHY_INFO(258, 0, 1, 0, 0),	//0x01020010,
		SET_PHY_INFO(223, 0, 1, 0, 0),  //0x00df0010, 
		SET_PHY_INFO(213, 0, 1, 0, 0),  //0x00d50010, 

		SET_PHY_INFO(162, 0, 4, 0, 0),	//0x00a20040,
		SET_PHY_INFO(127, 0, 4, 0, 0),	//0x007f0040,
		SET_PHY_INFO(117, 0, 4, 0, 0),  //0x00750040, 

		//G
		SET_PHY_INFO(60, 7, 7, 0, 0),  //0x003c0070,
		SET_PHY_INFO(52, 7, 7, 0, 0),  //0x00340070,
		SET_PHY_INFO(48, 9, 9, 0, 0),  //0x00300090,
		SET_PHY_INFO(44, 9, 9, 0, 0),  //0x002c0090,

		SET_PHY_INFO(44, 11, 11, 0, 0),  //0x002c00b0,
		SET_PHY_INFO(40, 11, 11, 0, 0),  //0x002800b0,
		SET_PHY_INFO(40, 11, 11, 0, 0),  //0x002800b0,
		SET_PHY_INFO(40, 11, 11, 0, 0),  //0x002800b0,

		//N
		SET_PHY_INFO(76,  7,  7, 0, 1),  //0x004c1c71, 
		SET_PHY_INFO(64,  9,  9, 1, 1),  //0x00402493, 
		SET_PHY_INFO(60,  9,  9, 2, 1),  //0x003c2495, 
		SET_PHY_INFO(60, 11, 11, 3, 1),  //0x003c2cb7, 
		SET_PHY_INFO(56, 11, 11, 4, 1),  //0x00382cb9, 
		SET_PHY_INFO(56, 11, 11, 5, 1),  //0x00382cbb, 
		SET_PHY_INFO(56, 11, 11, 5, 1),  //0x00382cbb, 
		SET_PHY_INFO(56, 11, 11, 5, 1),  //0x00382cbb, 


		SET_PHY_INFO(76,  7,  7, 6, 1),  //0x004c1c7d, 
		SET_PHY_INFO(64,  9,  9, 1, 1),  //0x00402493, 
		SET_PHY_INFO(60,  9,  9, 2, 1),  //0x003c2495, 
		SET_PHY_INFO(60, 11, 11, 3, 1),  //0x003c2cb7, 
		SET_PHY_INFO(56, 11, 11, 4, 1),  //0x00382cb9, 
		SET_PHY_INFO(56, 11, 11, 5, 1),  //0x00382cbb, 
		SET_PHY_INFO(56, 11, 11, 5, 1),  //0x00382cbb, 
		SET_PHY_INFO(56, 11, 11, 5, 1),  //0x00382cbb, 


		SET_PHY_INFO(64,  7,  7, 0, 0),  //0x00401c70, 
		SET_PHY_INFO(52,  9,  9, 0, 0),  //0x00342490, 
		SET_PHY_INFO(48,  9,  9, 0, 0),  //0x00302490, 
		SET_PHY_INFO(48, 11, 11, 0, 0),  //0x00302cb0, 
		SET_PHY_INFO(44, 11, 11, 0, 0),  //0x002c2cb0, 
		SET_PHY_INFO(44, 11, 11, 0, 0),  //0x002c2cb0, 
		SET_PHY_INFO(44, 11, 11, 0, 0),  //0x002c2cb0, 
		SET_PHY_INFO(44, 11, 11, 0, 0),  //0x002c2cb0, 

		/* PHY LL-Length Table: */
		SET_PHY_L_LENGTH(50, 38, 35),//0x000329a3
		SET_PHY_L_LENGTH(35, 29, 26),//0x0002375a
		SET_PHY_L_LENGTH(29, 26, 23),//0x0001d697
		SET_PHY_L_LENGTH(26, 23, 23),//0x0001a5d7
		SET_PHY_L_LENGTH(23, 23, 20),//0x000175d4

		SET_PHY_L_LENGTH(23, 20, 20),//0x00017514
		SET_PHY_L_LENGTH(47, 38, 35),//0x0002f9a3
		SET_PHY_L_LENGTH( 0,  0,  0),//0x00000000
	};
size_t phy_info_tbl_size = sizeof(phy_info_tbl);

//static u32 phy_info_tbl[] = 
//{
//    /* PHY Infor Table: */
//    0x00000000, 0x00000100, 0x00000200, 0x00000300, 0x00000140, 
//    0x00000240, 0x00000340, 0x00000001, 0x00000101, 0x00000201, 
//    0x00000301, 0x00000401, 0x00000501, 0x00000601, 0x00000701, 
//    0x00030002, 0x00030102, 0x00030202, 0x00030302, 0x00030402, 
//    0x00030502, 0x00030602, 0x00030702, 0x00030082, 0x00030182, 
//    0x00030282, 0x00030382, 0x00030482, 0x00030582, 0x00030682, 
//    0x00030782, 0x00030042, 0x00030142, 0x00030242, 0x00030342, 
//    0x00030442, 0x00030542, 0x00030642, 0x00030742,
//
//    /* PHY Index Table: */
//
////B	
//    0x013A0000, 0x01020010, 0x00df0010, 0x00d50010, 
//	0x00a20040, 0x007f0040, 0x00750040, 
////G
//	0x003c0070, 0x00340070, 0x00300090, 0x002c0090, 0x002c00b0, 0x002800b0, 0x002800b0, 0x002800b0, 
////N
//	0x004c1c71, 0x00402493, 0x003c2495, 0x003c2cb7, 0x00382cb9, 0x00382cbb, 0x00382cbb, 0x00382cbb, 
//	0x004c1c7d, 0x00402493, 0x003c2495, 0x003c2cb7, 0x00382cb9, 0x00382cbb, 0x00382cbb, 0x00382cbb, 
//	0x00401c70, 0x00342490, 0x00302490, 0x00302cb0, 0x002c2cb0, 0x002c2cb0, 0x002c2cb0, 0x002c2cb0,
//
//    /* PHY LL-Length Table: */
//    0x000329a3, 0x0002375a, 0x0001d697, 0x0001a5d7, 0x000175d4,
//    0x00017514, 0x0002f9a3, 0x00000000    
//};



#endif /* _DEV_TBL_H_ */

