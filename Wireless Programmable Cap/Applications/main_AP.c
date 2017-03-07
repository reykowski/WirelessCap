//******************************************************************************
// THIS PROGRAM IS PROVIDED "AS IS". TI MAKES NO WARRANTIES OR
// REPRESENTATIONS, EITHER EXPRESS, IMPLIED OR STATUTORY,
// INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR
// COMPLETENESS OF RESPONSES, RESULTS AND LACK OF NEGLIGENCE.
// TI DISCLAIMS ANY WARRANTY OF TITLE, QUIET ENJOYMENT, QUIET
// POSSESSION, AND NON-INFRINGEMENT OF ANY THIRD PARTY
// INTELLECTUAL PROPERTY RIGHTS WITH REGARD TO THE PROGRAM OR
// YOUR USE OF THE PROGRAM.
//
// IN NO EVENT SHALL TI BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
// CONSEQUENTIAL OR INDIRECT DAMAGES, HOWEVER CAUSED, ON ANY
// THEORY OF LIABILITY AND WHETHER OR NOT TI HAS BEEN ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGES, ARISING IN ANY WAY OUT
// OF THIS AGREEMENT, THE PROGRAM, OR YOUR USE OF THE PROGRAM.
// EXCLUDED DAMAGES INCLUDE, BUT ARE NOT LIMITED TO, COST OF
// REMOVAL OR REINSTALLATION, COMPUTER TIME, LABOR COSTS, LOSS
// OF GOODWILL, LOSS OF PROFITS, LOSS OF SAVINGS, OR LOSS OF
// USE OR INTERRUPTION OF BUSINESS. IN NO EVENT WILL TI'S
// AGGREGATE LIABILITY UNDER THIS AGREEMENT OR ARISING OUT OF
// YOUR USE OF THE PROGRAM EXCEED FIVE HUNDRED DOLLARS
// (U.S.$500).
//
// Unless otherwise stated, the Program written and copyrighted
// by Texas Instruments is distributed as "freeware".  You may,
// only under TI's copyright in the Program, use and modify the
// Program without any charge or restriction.  You may
// distribute to third parties, provided that you transfer a
// copy of this license to the third party and the third party
// agrees to these terms by its first use of the Program. You
// must reproduce the copyright notice and any other legend of
// ownership on each copy or partial copy, of the Program.
//
// You acknowledge and agree that the Program contains
// copyrighted material, trade secrets and other TI proprietary
// information and is protected by copyright laws,
// international copyright treaties, and trade secret laws, as
// well as other intellectual property laws.  To protect TI's
// rights in the Program, you agree not to decompile, reverse
// engineer, disassemble or otherwise translate any object code
// versions of the Program to a human-readable form.  You agree
// that in no event will you alter, remove or destroy any
// copyright notice included in the Program.  TI reserves all
// rights not specifically granted under this license. Except
// as specifically provided herein, nothing in this agreement
// shall be construed as conferring by implication, estoppel,
// or otherwise, upon you, any license or other right under any
// TI patents, copyrights or trade secrets.
//
// You may not use the Program in non-TI devices.
//
//******************************************************************************
//   eZ430-RF2500 Temperature Sensor Access Point
//
//   Description: This is the Access Point software for the eZ430-2500RF
//                Temperature Sensing demo
//
//
//   L. Westlund
//   Version    1.03
//   Texas Instruments, Inc
//   August 2009
//   Built with IAR Embedded Workbench Version: 4.20
//******************************************************************************
//Change Log:
//******************************************************************************
//Version:	1.04
//Comments: Added Enhanced Command Structure
//			Added Comments for Data Transfer
//Version:  1.03
//Comments: Added support for SimpliciTI 1.1.0 
//          Added support for Code Composer Studio
//          Added security (Enabled with -DSMPL_SECURE in smpl_nwk_config.dat)
//          Added acknowledgement (Enabled with -DAPP_AUTO_ACK in smpl_nwk_config.dat)
//          Based the modifications on the AP_as_Data_Hub example code
//Version:  1.02
//Comments: Changed Port toggling to abstract method
//          Removed ToggleLED
//          Fixed comment typos/errors
//          Changed startup string to 1.02
//Version:  1.01
//Comments: Added support for SimpliciTI 1.0.3
//          Changed RSSI read method 
//          Added 3 digit temperature output for 100+F
//          Changed startup string to 1.01
//Version:  1.00
//Comments: Initial Release Version

//**********************************************************************************
//					INCLUDES:
#include <string.h>
#include <stdbool.h>
#include "bsp.h"
#include "mrfi.h"
#include "bsp_leds.h"
#include "bsp_buttons.h"
#include "nwk_types.h"
#include "nwk_api.h"
#include "nwk_frame.h"
#include "nwk.h"
#include "virtual_com_cmds.h"
#include "nv_obj.h"
//#include "app_remap_led.h"
//******************************************************************************
//					DEFINITIONS:
#define SPIN_ABOUT_A_SECOND   NWK_DELAY(100)
#define SPIN_ABOUT_A_QUARTER_SECOND   NWK_DELAY(50)
#define SPIN_ABOUT_AN_EIGTH_SECOND   NWK_DELAY(25)
#define  CONNTABLEINFO_STRUCTURE_VERSION   1
#ifndef APP_AUTO_ACK
#error ERROR: Must define the macro APP_AUTO_ACK for this application.
#endif
//**********************************************************************************
// ---------------  Function Calls ----------------------
void toggleLED(uint8_t);
void bothLEDs_on();
void bothLEDs_off();
void toggle_bothLEDs();
void write_Values_to_Flash(void);
void read_Values_from_Flash(void);
void perform_self_Measurement(void);

/* received message handler */
static void processMessage(linkID_t, uint8_t *, uint8_t);
/* callback handler */
static uint8_t sCB(linkID_t);
/* Remote Address */
bool GetRemoteAddr( linkID_t* lid, addr_t* addr );
/* Frequency Agility helper functions */
static void    checkChangeChannel(void);
static void    changeChannel(void);

//**********************************************************************************
// ---------------  Interrupt Handlers ----------------------
__interrupt void ADC10_ISR(void);
__interrupt void Timer_A (void);
/******************************************************************************
 * TYPEDEFS
 */
/* This structure aggregates eveything necessary to save if we want to restore
 * the connection information later.
 */
typedef struct
{
  const uint8_t    structureVersion2; /* to dectect upgrades... */
        uint8_t    numConnections2;   /* count includes the UUD port/link ID */
/* The next two are used to detect overlapping port assignments. When _sending_ a
 * link frame the local port is assigned from the top down. When sending a _reply_
 * the assignment is bottom up. Overlapping assignments are rejected. That said it
 * is extremely unlikely that this will ever happen. If it does the test implemented
 * here is overly cautious (it will reject assignments when it needn't). But we leave
 * it that way on the assumption that it will never happen anyway.
 */
        uint8_t    curNextLinkPort2;
        uint8_t    curMaxReplyPort2;
        linkID_t   nextLinkID2;
#ifdef ACCESS_POINT
        sfInfo_t   sSandFContext2;
#endif
/* Connection table entries last... */
        connInfo_t connStruct2[NUM_CONNECTIONS+1];
} persistentContext_t2;

//**********************************************************************************
//-------------------- Variables -------------------
void *nwk_cfg_flash;
ioctlNVObj_t nwk_cfg_ram;
static persistentContext_t2 sPersistInfo2 = {CONNTABLEINFO_STRUCTURE_VERSION};
#define  SIZEOF_NV_OBJ2   sizeof(sPersistInfo2)
/* reserve space for the maximum possible peer Link IDs */
static linkID_t sLID[NUM_CONNECTIONS] = {0};
static smplStatus_t status;
static addr_t my_addr;
static addr_t remote_addr[NUM_CONNECTIONS] = {0};
static uint8_t  sNumCurrentPeers = 0;
static uint8_t sTxTid, sRxTid;
static uint8_t *data = NULL;
static uint8_t *data2 = NULL;
//data for terminal output
const char splash[] = {"\r\n--------------------------------------------------\r\n     ****\r\n     ****           eZ430-RF2500\r\n     ******o****    Temperature Sensor Network\r\n********_///_****   Copyright 2009\r\n ******/_//_/*****  Texas Instruments Incorporated\r\n  ** ***(__/*****   All rights reserved.\r\n      *********     SimpliciTI1.1.0\r\n       *****\r\n        ***\r\n--------------------------------------------------\r\n"};
static uint32_t JT = DEFAULT_JOIN_TOKEN;
static uint32_t JT2;
static int degC, volt;
static int results[2];

char LED_RED = 0;	// We need this to control the red LED on the ED's
char Device = 0;	// Device that needs to be addressed
char a[32];			// Character array
char index;
char num_con = NUM_CONNECTIONS;

/* work loop semaphores */
static volatile uint8_t sPeerFrameSem = 0;
static volatile uint8_t sJoinSem = 0;
static volatile uint8_t sSelfMeasureSem = 0;
/* blink LEDs when channel changes... */
static volatile uint8_t sBlinky = 0;
static volatile int * tempOffset = (int *)0x10F4; 
static volatile long temp;
//+++++++++++++++++++++++
#ifdef FREQUENCY_AGILITY
/*     ************** BEGIN interference detection support */
#define INTERFERNCE_THRESHOLD_DBM (-70)
#define SSIZE    25
#define IN_A_ROW  3
static int8_t  sSample[SSIZE];
static uint8_t sChannel = 0;
#endif  /* FREQUENCY_AGILITY */
//+++++++++++++++++++++++

static uint8_t rx_msg_ed[20];	// Message from End Device
//                            01234567890123456789
//                            00000000001111111111
//                          {"$CCCC,TTTT,VVVVVVVV#"}; // $Command, Target, Value#
static uint8_t tx_msg_ed[20] = {"$CLED,0001,00000002#"}; // Change LED #1 with Mode #2 (toggle)
static uint8_t rx_msg_hs[20];	// Message from Host
static uint8_t tx_msg_hs[20];	// Message to Host
static uint8_t addr[] = {"HUB0"};
static uint8_t rssi[] = {"000"};


void main (void)
{
  //uint8_t msg[20];
  bspIState_t intState;
  bool button1Pressed = false;

  JT2 = JT;
  memset(sSample, 0x0, sizeof(sSample));
  
  BSP_Init();
  
  nv_obj_init();

  if(BSP_BUTTON1())
  {
	bothLEDs_on();	
  	toggleLED(1);
  	nv_obj_erase_flash();		// Erase Flash
 	while ( BSP_BUTTON1())
    {
    	toggle_bothLEDs();
    	SPIN_ABOUT_AN_EIGTH_SECOND;   	// Wait
    }
  }


  BCSCTL3 |= LFXT1S_2;                      // LFXT1 = VLO
  TACCTL0 = CCIE;                           // TACCR0 interrupt enabled
  TACCR0 = 12000;                           // ~1 second
  TACTL = TASSEL_1 + MC_1;                  // ACLK, upmode  
  
  COM_Init();
  //Transmit splash screen and network init notification
  TXString( (char*)splash, sizeof splash);  
  TXString( "\r\nInitializing Network....", 26 );
  
  bothLEDs_off();
  SMPL_Init(sCB);
  
  SMPL_Ioctl(IOCTL_OBJ_ADDR, IOCTL_ACT_GET, &my_addr);
   
  // network initialized
  TXString( "Done\r\n", 6);
  
  /* green and red LEDs on solid to indicate waiting for a Join. */
  bothLEDs_on();

  nwk_cfg_flash = nv_obj_read_nwk_cfg();
  /* check for existing network configuration */
  if(nwk_cfg_flash != NULL)		/* Static Link Table in Flash */
  {
  	// Indicate start of reload
    bothLEDs_on();	
  	toggleLED(1);
 	for ( index=0;index < 10;index++)
    {
    	toggle_bothLEDs();
    	SPIN_ABOUT_AN_EIGTH_SECOND;   	// Wait
    }

	read_Values_from_Flash();
 	 
    // Indicate successfull reload
    bothLEDs_on();	
 	for ( index=0;index < 10;index++)
    {
    	toggle_bothLEDs();
    	SPIN_ABOUT_AN_EIGTH_SECOND;   	// Wait
    }    
  }  

  bothLEDs_on();



  /* main work loop */
  while (1)
  {
  	
	SPIN_ABOUT_A_SECOND;   	// Slow Down
	SPIN_ABOUT_A_SECOND;   	// Slow Down
	SPIN_ABOUT_A_SECOND;   	// Slow Down
	//SPIN_ABOUT_A_QUARTER_SECOND;   	// Slow Down
	
    /* Wait for the Join semaphore to be set by the receipt of a Join frame from a
     * device that supports an End Device.
     *
     * An external method could be used as well. A button press could be connected
     * to an ISR and the ISR could set a semaphore that is checked by a function
     * call here, or a command shell running in support of a serial connection
     * could set a semaphore that is checked by a function call.
     */
 
 
   /* check for existing network configuration */

  	//if(nwk_cfg_flash == NULL)
  	//{ 
    	if (sJoinSem && (sNumCurrentPeers < NUM_CONNECTIONS))
    	// At least one device joined the network - check if we can link to it
    	{
    		/* listen for a new connection */
    		bothLEDs_on();
    		//index=0;
    		if (nwk_cfg_flash == NULL) // No stored link table
    		{   // Allow additional devices to link
      			while (1)
      			{   
      				toggleLED(1);
	    			// if (SMPL_SUCCESS == SMPL_Link(&sLID[sNumCurrentPeers]))
	    			if (SMPL_SUCCESS == SMPL_LinkListen(&sLID[sNumCurrentPeers]))
        			{
           				sNumCurrentPeers++;		// Link was successfull, increase # of peers
           				break;
           				/* Implement fail-to-link policy here. otherwise, listen again. */
        			}
        			SPIN_ABOUT_A_QUARTER_SECOND;
        			//index++;
        			//if (index>10)
        			//{
        			//	break;			// Too many attempts - let's leave
        			//}	
      			}
      			//SMPL_SUCCESS == SMPL_Unlink(sLID[sNumCurrentPeers+1]);
    		}  
      		BSP_ENTER_CRITICAL_SECTION(intState);
      		sJoinSem--;				// Reduce the join semaphore
      		BSP_EXIT_CRITICAL_SECTION(intState);
      		
  			/* Turn both LEDs off after link */
  			bothLEDs_off();
    	}
  	//}


    
    // if it is time to measure our own temperature...
    if(sSelfMeasureSem)
    {
      perform_self_Measurement();
    }    
    
    /* Have we received a frame on one of the ED connections?
     * No critical section -- it doesn't really matter much if we miss a poll
     */
    
    
    if (sNumCurrentPeers>0)
    {
		/* turn on RX. default is RX off. */
		SMPL_Ioctl( IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_RXON, 0);
		SMPL_Send(sLID[Device], tx_msg_ed, sizeof(tx_msg_ed));   // Send message to End Device
		toggleLED(2);					// Turn on Red LED
		SPIN_ABOUT_A_QUARTER_SECOND;   	// Wait
		SPIN_ABOUT_A_QUARTER_SECOND;   	// wait
		bothLEDs_off();					// Turn off both LED

		if (Device<sNumCurrentPeers-1)
		{
					Device++;
		}
		else
		{
					Device=0;
		}		


    }
    

    if((BSP_BUTTON1()) && (button1Pressed == false))
    {
       BSP_Delay(20000); // debouncing delay
      
       if(BSP_BUTTON1())
       {
         // set flag
         button1Pressed = true;
                
         write_Values_to_Flash();
                  
		 SPIN_ABOUT_A_QUARTER_SECOND;   	// Slow Down

//++++++++++++Test		 
    	 read_Values_from_Flash();

         while ( BSP_BUTTON1())
    	 {
    		toggle_bothLEDs();
    		SPIN_ABOUT_AN_EIGTH_SECOND;   	// Wait
    	 }
         
         SPIN_ABOUT_A_QUARTER_SECOND;   	// Slow Down

		 bothLEDs_off();
//+++++++++++++++++		 
		 
         
       }
    }
    else if((!BSP_BUTTON1()) && (button1Pressed == true))
    {
       // clear flag
       button1Pressed = false;
       SPIN_ABOUT_A_QUARTER_SECOND;   	// Slow Down
    }   		
	 
  }
}

void toggleLED(uint8_t which)
{
  if (1 == which)
  {
    BSP_TOGGLE_LED1();
  }
  else if (2 == which)
  {
    BSP_TOGGLE_LED2();
  }

  return;
}

void bothLEDs_on()
{
	if (!BSP_LED2_IS_ON())
  	{
    	toggleLED(2);
  	}
  	if (!BSP_LED1_IS_ON())
  	{
    	toggleLED(1);
  	}  	
}

void bothLEDs_off()
{
	if (BSP_LED2_IS_ON())
  	{
    	toggleLED(2);
  	}
  	if (BSP_LED1_IS_ON())
  	{
    	toggleLED(1);
  	}  	
}

void toggle_bothLEDs()
{
    BSP_TOGGLE_LED1();
    BSP_TOGGLE_LED2();
}




/* Runs in ISR context. Reading the frame should be done in the */
/* application thread not in the ISR thread. */
static uint8_t sCB(linkID_t lid)
// Call Back Function defined during "SMPL_Init(sCB)"
// Same Call Back Function as spCallback in nwk_frame.c

{
  if (lid)
  {
    sPeerFrameSem++;
    //sBlinky = 0;
  }
  else
  {
    sJoinSem++;
    bothLEDs_off();
    toggleLED(2);
  }

  /* leave frame to be read by application. */
  return 0;
}

static void processMessage(linkID_t lid, uint8_t *msg, uint8_t len)
{
  /* do something useful */
  if (len)
  {
    //toggleLED(*msg);
  }
  return;
}

static void changeChannel(void)
{
#ifdef FREQUENCY_AGILITY
  freqEntry_t freq;

  if (++sChannel >= NWK_FREQ_TBL_SIZE)
  {
    sChannel = 0;
  }
  freq.logicalChan = sChannel;
  SMPL_Ioctl(IOCTL_OBJ_FREQ, IOCTL_ACT_SET, &freq);
  //BSP_TURN_OFF_LED1();
  BSP_TURN_OFF_LED2();
  sBlinky = 1;
#endif
  return;
}

/* implement auto-channel-change policy here... */
static void  checkChangeChannel(void)
{
#ifdef FREQUENCY_AGILITY
  int8_t dbm, inARow = 0;

  uint8_t i;

  memset(sSample, 0x0, SSIZE);
  for (i=0; i<SSIZE; ++i)
  {
    /* quit if we need to service an app frame */
    if (sPeerFrameSem || sJoinSem)
    {
      return;
    }
    NWK_DELAY(1);
    SMPL_Ioctl(IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_RSSI, (void *)&dbm);
    sSample[i] = dbm;

    if (dbm > INTERFERNCE_THRESHOLD_DBM)
    {
      if (++inARow == IN_A_ROW)
      {
        changeChannel();
        break;
      }
    }
    else
    {
      inARow = 0;
    }
  }
#endif
  return;
}


//-----------------------------------------------------------------------------
// possible required include files

//#include "nwk.h"

bool GetRemoteAddr( linkID_t* lid, addr_t* addr )
{
  connInfo_t *pCInfo = nwk_getConnInfo(*lid);



  // if a connection object found and address pointer not null
  if( pCInfo && addr )
  { // copy over the address found to the supplied structure
    memcpy( addr->addr, pCInfo->peerAddr, NET_ADDR_SIZE );

    return true; // indicate success
  }

  // no connection found, indicate failure
  return false; 
} 


void read_Values_from_Flash(void)
{
  data2 = (uint8_t *)&sPersistInfo2; 
  /* point to pointer */
  nwk_cfg_ram.objPtr = &data;
  nwk_cfg_flash = nv_obj_read_nwk_cfg();
  
  /* restore network configuratio to RAM */
  if(SMPL_Ioctl(IOCTL_OBJ_NVOBJ, IOCTL_ACT_GET, &nwk_cfg_ram) == SMPL_SUCCESS)
  {
    memcpy(data, nwk_cfg_flash, nwk_cfg_ram.objLen);
    memcpy(data2, nwk_cfg_flash, nwk_cfg_ram.objLen);
  }

  /* get link ID from flash */
  for ( index=0;index < NUM_CONNECTIONS;index++)
  {
  	sLID[index] = nv_obj_read_lnk_id(index);
  }
  /* get number of peers from flash */
  sNumCurrentPeers = nv_obj_read_lnk_id(NUM_CONNECTIONS);   
}


void write_Values_to_Flash(void)
{
	/* point to pointer */
    nwk_cfg_ram.objPtr = &data;
        
	// save network configuration in non volatile memory
	if(SMPL_Ioctl(IOCTL_OBJ_NVOBJ, IOCTL_ACT_GET, &nwk_cfg_ram) == SMPL_SUCCESS)
	{
		if(nv_obj_write_nwk_cfg(data,nwk_cfg_ram.objLen) != true)
		{
			__no_operation(); // for debugging
		}
		//memcpy(data2, data, nwk_cfg_ram.objLen);
	}
         
	// save link ID in non volatile memory
	for ( index=0 ; index < NUM_CONNECTIONS ; index++)
	{
		if(nv_obj_write_lnk_id(index, sLID[index]) == false)
		{
			__no_operation(); // for debugging
		}
	}

	index = NUM_CONNECTIONS;
	// Little cheat: write number of current peers //
	if(nv_obj_write_lnk_id(index, sNumCurrentPeers) == false)
	{
		__no_operation(); // for debugging
	}
}



void perform_self_Measurement(void)
{
      
   ADC10CTL1 = INCH_10 + ADC10DIV_4;     // Temp Sensor ADC10CLK/5
   ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + ADC10IE + ADC10SR;
   for( degC = 240; degC > 0; degC-- );  // delay to allow reference to settle
   ADC10CTL0 |= ENC + ADC10SC;           // Sampling and conversion start
   __bis_SR_register(CPUOFF + GIE);      // LPM0 with interrupts enabled
   results[0] = ADC10MEM;
  
   ADC10CTL0 &= ~ENC;
  
   ADC10CTL1 = INCH_11;                  // AVcc/2
   ADC10CTL0 = SREF_1 + ADC10SHT_2 + REFON + ADC10ON + ADC10IE + REF2_5V;
   for( degC = 240; degC > 0; degC-- );  // delay to allow reference to settle
   ADC10CTL0 |= ENC + ADC10SC;           // Sampling and conversion start
   __bis_SR_register(CPUOFF + GIE);      // LPM0 with interrupts enabled
   results[1] = ADC10MEM;
   ADC10CTL0 &= ~ENC;
   ADC10CTL0 &= ~(REFON + ADC10ON);      // turn off A/D to save power
     
   // oC = ((A10/1024)*1500mV)-986mV)*1/3.55mV = A10*423/1024 - 278
   // the temperature is transmitted as an integer where 32.1 = 321
   // hence 4230 instead of 423
   temp = results[0];
   degC = (((temp - 673) * 4230) / 1024);
   if( (*tempOffset) != 0xFFFF )
   {
     degC += (*tempOffset); 
   }
      
   temp = results[1];
   volt = (temp*25)/512;
    
   tx_msg_hs[0] = degC&0xFF;
   tx_msg_hs[1] = (degC>>8)&0xFF;
   tx_msg_hs[2] = volt;
   transmitDataString(1, addr, rssi, tx_msg_hs );	// Send message to host
   // "transmitDataString" = Assemble the Access Point data string to be send to the host.
   // "MESSAGE Length" is defined in virtual_com_cmds.h: e.g. #define MESSAGE_LENGTH 3
   //BSP_TOGGLE_LED1();
   sSelfMeasureSem = 0;
}



/*------------------------------------------------------------------------------
* ADC10 interrupt service routine
------------------------------------------------------------------------------*/
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}

/*------------------------------------------------------------------------------
* Timer A0 interrupt service routine
------------------------------------------------------------------------------*/
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
  sSelfMeasureSem = 1;
}

