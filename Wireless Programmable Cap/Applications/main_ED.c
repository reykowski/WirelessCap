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
//   eZ430-RF2500 Temperature Sensor End Device
//
//   Description: This is the End Device software for the eZ430-2500RF
//                Temperature Sensing demo
//
//
//   L. Westlund
//   Version    1.02
//   Texas Instruments, Inc
//   November 2007
//   Built with IAR Embedded Workbench Version: 4.09A
//******************************************************************************
//Change Log:
//******************************************************************************
//Version:  1.03
//Comments: Added support for SimpliciTI 1.1.0 
//          Added support for Code Composer Studio
//          Added security (Enabled with -DSMPL_SECURE in smpl_nwk_config.dat)
//          Added acknowledgement (Enabled with -DAPP_AUTO_ACK in smpl_nwk_config.dat)
//          Based the modifications on the AP_as_Data_Hub example code
//Version:  1.02
//Comments: Changed Port toggling to abstract method
//          Fixed comment typos
//Version:  1.01
//Comments: Added support for SimpliciTI 1.0.3
//          Added Flash storage/check of Random address
//          Moved LED toggle to HAL
//Version:  1.00
//Comments: Initial Release Version
//******************************************************************************
//					DEFINITIONS:
#define I_WANT_TO_CHANGE_DEFAULT_ROM_DEVICE_ADDRESS_PSEUDO_CODE
#define SPIN_ABOUT_A_SECOND   NWK_DELAY(100)
#define SPIN_ABOUT_A_QUARTER_SECOND   NWK_DELAY(50)
#define SPIN_ABOUT_AN_EIGTH_SECOND   NWK_DELAY(25)
#define NOC    5	// Number of Commands in Lookup-Table
/* How many times to try a Tx and miss an acknowledge before doing a scan */
#define MISSES_IN_A_ROW  2
#ifndef APP_AUTO_ACK
#error ERROR: Must define the macro APP_AUTO_ACK for this application.
#endif
//**********************************************************************************
//					INCLUDES:
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "bsp.h"
#include "mrfi.h"
#include "nwk_types.h"
#include "nwk_api.h"
#include "bsp_leds.h"
#include "bsp_buttons.h"
#include "vlo_rand.h"
#include "nwk.h"
#include "nv_obj.h"
//**********************************************************************************
// ---------------  Function Calls ----------------------
void toggleLED(uint8_t);
void bothLEDs_on(void);
void bothLEDs_off(void);
void toggle_bothLEDs(void);
void createRandomAddress(void); 
void storeLinkInfo(void);
void eraseLinkInfo(void);
void write_Values_to_SPI(void);
void write_Values_to_Flash(void);
void read_Values_from_Flash(void);
void perform_self_Measurement(void);
void parseCommandString(uint8_t* msg);
bool GetRemoteAddr( linkID_t* lid, addr_t* addr );
static void linkTo(void);
/* Rx callback handler */
static uint8_t sRxCallback(linkID_t);
//**********************************************************************************
// ---------------  Interrupt Calls ----------------------
__interrupt void ADC10_ISR(void);
__interrupt void Timer_A (void);
//**********************************************************************************
//-------------------- Variables -------------------
void *nwk_cfg_flash;

static ioctlNVObj_t nwk_cfg_ram;
static linkID_t sLinkID1 = 0;
static addr_t remote_addr;
static uint8_t *data = NULL;
static uint8_t locPort;
static uint8_t rmtPort;
static uint8_t rx_msg[20];
static uint8_t rx_msg_tmp[20];	// Temp buffer for latest rx frame
static uint8_t tx_msg[20];
static int high_val;
static int low_val;
static int target_val;
static char fc;
static uint32_t JT = DEFAULT_JOIN_TOKEN;
static uint32_t JT2;
static uint32_t LT = DEFAULT_LINK_TOKEN;
static uint32_t LT2;

/* work loop semaphores */
static volatile uint8_t sPeerFrameSem = 0;
static volatile uint8_t sJoinSem = 0;
static volatile uint8_t txSemaphore =0;
static volatile uint8_t rxSemaphore = 0;	// Receive buffer ready semaphore
static volatile uint8_t sSelfMeasureSem = 0;

volatile long temp;
volatile int * tempOffset = (int *)0x10F4; // Temperature offset set at production
volatile char C1;		// Capacitor Value 0-31

int degC, volt;
int results[2];      


char num_con = NUM_CONNECTIONS;
char index;
char * Flash_Addr = (char *)0x10F0;          // Initialize radio address location
char * Flash_Addr2 = (char *)0x1000;         // Initialize connection info location
//char * FlashBuffer[256];

//		       	      0123456
uint8_t cmd[] = 	{"CCCC"};
uint8_t target[] =	{"TTTT"};
uint8_t value[] =   {"VVVVVVVV"};
//		       	      01234567

// Command Structure between Host and Access Point (AP):
char *cmd_table[NOC] = {"SCPV",
					    "GCPV",
					    "SDSM",
					    "GDST",
					    "CLED"};
// SCPV = Set Capacitor Value
// GCPV = Get Capacitor Value
// SDSM = Set Device to Sleep Mode
// GDST = Get Device Status
// CLED = Change LED Status

//                    01234567890123456789
//                    00000000001111111111
char io_command[] = {"$CCCC,TTTT,VVVVVVVV#"}; // $Command, Target, Value#

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 						Main Routine
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


void main (void)
{ 
  addr_t lAddr;
  
  SPIN_ABOUT_A_SECOND;
  
  BSP_Init();
  
  nv_obj_init();
  
  // Initialize SPI enable lines:
  
  P2DIR |= BIT4;			// P2.4 is output
  P4DIR |= BIT4;			// P4.4 is output
  P4DIR |= BIT6;			// P4.6 is output

  P2OUT &= !BIT4;			// P2.4 is low
  P4OUT &= !BIT4;			// P4.4 is low
  P4OUT &= !BIT6;			// P4.6 is low 
  
  if(BSP_BUTTON1())
  {
	bothLEDs_on();	
  	toggleLED(1);
  	nv_obj_erase_flash();		// Erase Flash
 	while ( BSP_BUTTON1())
    {
    	toggle_bothLEDs();
    	SPIN_ABOUT_AN_EIGTH_SECOND;   	// Toggle both lights to indicate erase
    }
  }
  
  JT2 = JT;
  LT2 = LT;

  if(Flash_Addr[0] == 0xFF && Flash_Addr[1] == 0xFF && 
     Flash_Addr[2] == 0xFF && Flash_Addr[3] == 0xFF ) 
  {
    createRandomAddress(); // set Random device address at initial startup
  }
  lAddr.addr[0] = Flash_Addr[0];
  lAddr.addr[1] = Flash_Addr[1];
  lAddr.addr[2] = Flash_Addr[2];
  lAddr.addr[3] = Flash_Addr[3];
  SMPL_Ioctl(IOCTL_OBJ_ADDR, IOCTL_ACT_SET, &lAddr);

  /* Keep trying to join (a side effect of successful initialization) until
   * successful. Toggle LEDS to indicate that joining has not occurred.
   */
  bothLEDs_off();
  index=0;
  while (SMPL_SUCCESS != SMPL_Init(sRxCallback))
  {
    toggle_bothLEDs();
    SPIN_ABOUT_AN_EIGTH_SECOND;
    index++;
    //if (index==10)
    //{
    //	break;
    //}
  }

  /* LEDs on solid to indicate successful join. */
  bothLEDs_on();

  BCSCTL3 |= LFXT1S_2;                      // LFXT1 = VLO
  TACCTL0 = CCIE;                           // TACCR0 interrupt enabled
  TACCR0 = 12000;                           // ~ 1 sec
  TACTL = TASSEL_1 + MC_1;                  // ACLK, upmode  
  
  /* Unconditional link to AP which is sending link due to successful join. */
  linkTo();

  while (1) ;
}

static void linkTo()
{
  
  bool button1Pressed = false;


  /* check if an existing network configuration exists */
  nwk_cfg_flash = nv_obj_read_nwk_cfg();
  if(nwk_cfg_flash == NULL)
  {
	bothLEDs_on();	
  	toggleLED(1);
  	/* Keep trying to link... */
  	//while (SMPL_SUCCESS != SMPL_LinkListen(&sLinkID1))
  	while (SMPL_SUCCESS != SMPL_Link(&sLinkID1))
  	{
		toggle_bothLEDs();
    	SPIN_ABOUT_A_QUARTER_SECOND;
  	}
  	/* Link established - Turn off LEDs. */
	bothLEDs_off();

	/* Get connection info 
   	connInfo_t *pCInfo = nwk_getConnInfo(sLinkID1);
  	memcpy(&remote_addr, &pCInfo->peerAddr, NET_ADDR_SIZE);
  	locPort = pCInfo->portRx;
  	rmtPort = pCInfo->portTx;
 	*/  	
  }	
  else
  {

  	// Indicate start of reload
    bothLEDs_on();	
  	toggleLED(1);
 	for ( index=0;index < 10;index++)
    {
    	toggle_bothLEDs();
    	SPIN_ABOUT_AN_EIGTH_SECOND;   	// Wait
    }
	
	//memcpy(FlashBuffer, Flash_Addr2, 256);		// Copy from Flash Address into Flash Buffer

	read_Values_from_Flash();  
    
    // Indicate successfull reload
    bothLEDs_on();	
 	for ( index=0;index < 10;index++)
    {
    	toggle_bothLEDs();
    	SPIN_ABOUT_AN_EIGTH_SECOND;   	// Wait
    }    
    
    
  }


  /* Turn both LEDs off after link */
  bothLEDs_off();

  while (1)   /* Main Loop */
  {

 
    __bis_SR_register(LPM3_bits+GIE);  // LPM3 with interrupts enabled    


    if (sSelfMeasureSem) 
    {
      perform_self_Measurement();
    }


		/* Check for a frame to be received. The Rx handler, which is running in
		* ISR thread, will post to this semaphore allowing the application to
		* send the reply message in the user thread.
		*/
		SMPL_Ioctl( IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_RXON, 0);

		if (rxSemaphore)		// RX Message ready?
		{
			toggleLED(rx_msg[9]-48);
			if (C1<31)
			{
				C1=C1+1;
			}
			else
			{
				C1=0;
			}
			write_Values_to_SPI();
			
			/* Reset semaphore. This is not properly protected and there is a race
			* here. In theory we could miss a message. Good enough for a demo, though.
			*/
			parseCommandString(&rx_msg[0]);
			rxSemaphore = 0;	
		}


		if (txSemaphore)		// TX Message ready?
		{
			SMPL_Send(sLinkID1, tx_msg, sizeof(tx_msg));
			/* Reset semaphore. This is not properly protected and there is a race
			* here. In theory we could miss a message. Good enough for a demo, though.
			*/
			txSemaphore = 0;
		}
	
		
		// GetRemoteAddr( &sLinkID1, &remote_addr );
		
     	if((BSP_BUTTON1()) && (button1Pressed == false))
     	{
       		BSP_Delay(20000); // debouncing delay
      
       		if(BSP_BUTTON1())
       		{
         		// set flag
         		button1Pressed = true;

				write_Values_to_Flash();     
         		
 				while ( BSP_BUTTON1())
    			{
    				toggle_bothLEDs();
    				SPIN_ABOUT_AN_EIGTH_SECOND;   	// Toggle both lights to indicate write
    			}


//+++++++++++++Test
				read_Values_from_Flash();  
				/*
				//memcpy(FlashBuffer, Flash_Addr2, 256);	// Copy from Flash Address into Flash Buffert
    			// point to pointer 
    			nwk_cfg_ram.objPtr = &data;
				nwk_cfg_flash = nv_obj_read_nwk_cfg();
    			// restore network configuratio to RAM 
    			if(SMPL_Ioctl(IOCTL_OBJ_NVOBJ, IOCTL_ACT_GET, &nwk_cfg_ram) == SMPL_SUCCESS)
    			{
     			 	memcpy(data, nwk_cfg_flash, nwk_cfg_ram.objLen);
    			}
    			// get link ID from flash 
    			sLinkID1 = nv_obj_read_lnk_id(0);

				*/
				
				bothLEDs_off();
				
         		      		
       		}
     	}
     	else if((!BSP_BUTTON1()) && (button1Pressed == true))
     	{
       		// clear flag
       		button1Pressed = false;
     	}   		
	
  }
   
}


/* Runs in ISR context. Reading the frame should be done in the */
/* application thread not in the ISR thread. */
static uint8_t sRxCallback(linkID_t port)
// Call Back Function defined during "SMPL_Init(sCB)"
// Same Call Back Function as spCallback in nwk_frame.c
//                            01234567890123456789
//                            00000000001111111111
//                          {"$CCCC,TTTT,VVVVVVV# "}; // $Command, Target, Value#
//                          {"$CLED,0001,0000002# "}; // Change LED #1 with Mode #2 (toggle)
{
	uint8_t len;
	/* is the callback for the link ID we want to handle? */
	if (port == sLinkID1)
	{
		/* yes. go get the frame. we know this call will succeed. */
		if ((SMPL_SUCCESS == SMPL_Receive(sLinkID1, rx_msg_tmp, &len)) && len)
		{
			if (rxSemaphore == 0)	// RX buffer is empty
			{
				memcpy(rx_msg,rx_msg_tmp,sizeof(rx_msg));	// Move received string into buffer
				rxSemaphore = 1;
			}

			/* drop frame. we're done with it. */
			return 1;
		}
	}
    /* keep frame for later handling */
	return 0;
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


void createRandomAddress()
{
  unsigned int rand, rand2;
  do
  {
    rand = TI_getRandomIntegerFromVLO();    // first byte can not be 0x00 of 0xFF
  }
  while( (rand & 0xFF00)==0xFF00 || (rand & 0xFF00)==0x0000 );
  rand2 = TI_getRandomIntegerFromVLO();
  
  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO to 1MHz
  DCOCTL = CALDCO_1MHZ;
  FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 for Flash Timing Generator
  FCTL3 = FWKEY + LOCKA;                    // Clear LOCK & LOCKA bits
  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation
  
  Flash_Addr[0]=(rand>>8) & 0xFF;
  Flash_Addr[1]=rand & 0xFF;
  Flash_Addr[2]=(rand2>>8) & 0xFF; 
  Flash_Addr[3]=rand2 & 0xFF; 
  
  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCKA + LOCK;             // Set LOCK & LOCKA bit
}


void storeLinkInfo()
{
  
  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO to 1MHz
  DCOCTL = CALDCO_1MHZ;
  FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 for Flash Timing Generator
  FCTL3 = FWKEY + LOCKA;                    // Clear LOCK & LOCKA bits
  
  
  FCTL1 = FWKEY + ERASE;                    // Set Erase bit
  
  Flash_Addr2[0]=0;							// Erase Code Segment
  
  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation
  
  Flash_Addr2[0]=0x47;						// Check Code
  Flash_Addr2[1]=0x31;						// Check Code
  Flash_Addr2[2]=locPort; 
  Flash_Addr2[3]=rmtPort; 
  
  Flash_Addr2[4]=remote_addr.addr[0];
  Flash_Addr2[5]=remote_addr.addr[1];
  Flash_Addr2[6]=remote_addr.addr[2];
  Flash_Addr2[7]=remote_addr.addr[3];
    
  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCKA + LOCK;             // Set LOCK & LOCKA bit
}

void eraseLinkInfo()
{
  
  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO to 1MHz
  DCOCTL = CALDCO_1MHZ;
  FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 for Flash Timing Generator
  FCTL3 = FWKEY + LOCKA;                    // Clear LOCK & LOCKA bits
  
  
  FCTL1 = FWKEY + ERASE;                    // Set Erase bit
  
  Flash_Addr2[0]=0;							// Erase Code Segment
  
  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCKA + LOCK;             // Set LOCK & LOCKA bit
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


void parseCommandString(uint8_t* msg)
// Parse Buffer
//                      01234567890123456789
//                      00000000001111111111
//rx_msg[] = 		  {"$CCCC,TTTT,VVVVVVVV# "}; // $Command, Target, Value#
//                      00000000001111111111
//                      01234567890123456789
//char cmd[] = 	      {"CCCC"};
//                      0123
//char target[] =     {"TTTT"};
//                      0123
//char value[] =      {"VVVVVVVV"};
//                      01234567
{
	cmd[0] = msg[1];
	cmd[1] = msg[2];
	cmd[2] = msg[3];
	cmd[3] = msg[4];

	target[0] = msg[6];
	target[1] = msg[7];
	target[2] = msg[8];
	target[3] = msg[9];

	value[0] = msg[11];
	value[1] = msg[12];
	value[2] = msg[13];
	value[3] = msg[14];
	value[4] = msg[15];
	value[5] = msg[16];
	value[6] = msg[17];
	value[7] = msg[18];
	
	target_val=0;
	for (index = 0;index<4;index++)
	{
		target_val = target_val*10+(target[index]-48);
	}

	high_val=0;
	low_val=0;
	for (index = 0;index<4;index++)
	{
		high_val = high_val*10+(value[index]-48);
	}	
	for (index = 4;index<8;index++)
	{
		low_val = low_val*10+(value[index]-48);
	}		
	
	fc=0;

	for (index=0; index<NOC; index++)
	{
		if( strcmp((const char *) cmd,(const char *) cmd_table[index])==0)
		{
		fc = index+1;	
		}
		
	}
}


void write_Values_to_SPI()
{
	
	__disable_interrupt();
	// Write to C1:
	P2OUT |= BIT4;		 	/*Pull P2.4 high (SPI Select Line) */
	UCB0TXBUF=C1;
	_delay_cycles(1000);	// 1 milli seconds	at 1MHz clock
	P2OUT &= ~BIT4; 			/*Clear P2.4 (SPI Select Line) */
	
/*	
 	// Write to C2:
	P4OUT |= BIT4;		 	//Pull P4.4 high (SPI Select Line) 
	UCB0TXBUF=C2;
	_delay_cycles(1000);	// 1 milli seconds	at 1MHz clock
	P4OUT &= ~BIT4; 		//Clear P4.4 (SPI Select Line) 
	
	// Write to C3:
	P4OUT |= BIT6;		 	// Pull P4.6 high (SPI Select Line) 
	UCB0TXBUF=C3;
	_delay_cycles(1000);	// 1 milli seconds	at 1MHz clock
	P4OUT &= ~BIT6; 		//Clear P6.6 (SPI Select Line) 	

*/	
	__enable_interrupt();	
}

void write_Values_to_Flash()
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
	}
         
	// save link ID in non volatile memory
	if(nv_obj_write_lnk_id(0, sLinkID1) == false)
	{
   			__no_operation(); // for debugging
	}

	// Little cheat: write capacitor value C1 //
	if(nv_obj_write_lnk_id(1, C1) == false)
	{
		__no_operation(); // for debugging
	}
}         		

void read_Values_from_Flash(void)
{ 
    /* point to pointer */
    nwk_cfg_ram.objPtr = &data;
    
    /* restore network configuratio to RAM */
    if(SMPL_Ioctl(IOCTL_OBJ_NVOBJ, IOCTL_ACT_GET, &nwk_cfg_ram) == SMPL_SUCCESS)
    {
      memcpy(data, nwk_cfg_flash, nwk_cfg_ram.objLen);
    }
    
    /* get link ID from flash */
    sLinkID1 = nv_obj_read_lnk_id(0);
    
    /* get capacitor value from flash */
    C1 = nv_obj_read_lnk_id(1);
}





void perform_self_Measurement()
{
      
    /* get radio ready...awakens in idle state */
    //SMPL_Ioctl( IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_AWAKE, 0);
            
    ADC10CTL1 = INCH_10 + ADC10DIV_4;       // Temp Sensor ADC10CLK/5
    ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + ADC10IE + ADC10SR;
    for( degC = 240; degC > 0; degC-- );    // delay to allow reference to settle
    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
    __bis_SR_register(CPUOFF + GIE);        // LPM0 with interrupts enabled

    results[0] = ADC10MEM;    
    ADC10CTL0 &= ~ENC;    
    ADC10CTL1 = INCH_11;                     // AVcc/2
    ADC10CTL0 = SREF_1 + ADC10SHT_2 + REFON + ADC10ON + ADC10IE + REF2_5V;
    for( degC = 240; degC > 0; degC-- );    // delay to allow reference to settle
    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
    __bis_SR_register(CPUOFF + GIE);        // LPM0 with interrupts enabled
    results[1] = ADC10MEM;
    ADC10CTL0 &= ~ENC;
    ADC10CTL0 &= ~(REFON + ADC10ON);        // turn off A/D to save power
    
    // oC = ((A10/1024)*1500mV)-986mV)*1/3.55mV = A10*423/1024 - 278
    // the temperature is transmitted as an integer where 32.1 = 321
    // hence 4230 instead of 423
    temp = results[0];
    degC = ((temp - 673) * 4230) / 1024;
    if( (*tempOffset) != 0xFFFF )
    {
      degC += (*tempOffset); 
    }
    /* message format,  UB = upper Byte, LB = lower Byte
    -------------------------------
    |degC LB | degC UB |  volt LB |
    -------------------------------
       0         1          2
    */
    
    temp = results[1];
    volt = (temp*25)/512;
    tx_msg[0] = degC&0xFF;				// Load up data for transmission to AP
    tx_msg[1] = (degC>>8)&0xFF;
    tx_msg[2] = volt;
 
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
  __bic_SR_register_on_exit(LPM3_bits);        // Clear LPM3 bit from 0(SR)
}
