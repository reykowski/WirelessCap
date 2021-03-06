/*----------------------------------------------------------------------------
 *  Demo Application for SimpliciTI
 *
 *  L. Friedman
 *  Texas Instruments, Inc.
 *----------------------------------------------------------------------------
 */

/**********************************************************************************************
  Copyright 2007-2009 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights granted under
  the terms of a software license agreement between the user who downloaded the software,
  his/her employer (which must be your employer) and Texas Instruments Incorporated (the
  "License"). You may not use this Software unless you agree to abide by the terms of the
  License. The License limits your use, and you acknowledge, that the Software may not be
  modified, copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio frequency
  transceiver, which is integrated into your product. Other than for the foregoing purpose,
  you may not use, reproduce, copy, prepare derivative works of, modify, distribute,
  perform, display or sell this Software and/or its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS�
  WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY
  WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
  IN NO EVENT SHALL TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE
  THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY
  INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST
  DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY
  THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/
#include <string.h>
#include "bsp.h"
#include "virtual_com_cmds.h"

static char verboseMode = 1; 
static char degCMode = 0; 
static char buffer_active_flag = 0;
static char buffer_full_flag = 0;
static char buffer_error_flag = 0;
static char buffer_start_pointer = 0;
static char buffer_end_pointer = 0;

static char LookUpTable(void);
//char LED_RED = 0;	// We need this to control the red LED on the ED's

//		       	  01234567
char cmd[] = 	{"CCCC"};
char target[] =	{"TTTT"};
char value[] =  {"VVVVVVVV"};
//		       	  01234567
#define NOC    4	// Number of Commands in Lookup-Table
// Command Structure between Host and Access Point (AP):
char *cmd_table[NOC] = {"SCPV",
					    "GCPV",
					    "SDSM",
					    "GDST"};
// SCPV = Set Capacitor Value
// GCPV = Get Capacitor Value
// SDSM = Set Device to Sleep Mode
// GDST = Get Device Status


//                    01234567890123456789
//                    00000000001111111111
char io_command[] = {"$CCCC,TTTT,VVVVVVVV#"}; // $Command, Target, Value#

/******************************************************************************/
// End Virtual Com Port Communication
/******************************************************************************/
void COM_Init(void)
{  
  P3SEL |= 0x30;                            // P3.4,5 = USCI_A0 TXD/RXD
  UCA0CTL1 = UCSSEL_2;                      // SMCLK
  UCA0BR0 = 0x41;                           // 9600 from 8Mhz
  UCA0BR1 = 0x3;
  UCA0MCTL = UCBRS_2;                       
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
  __enable_interrupt(); 
}

void TXString( char* string, int length )   // Send data through COM Port to Host
{
  int pointer;
  for( pointer = 0; pointer < length; pointer++)
  {
    volatile int i;
    UCA0TXBUF = string[pointer];
    while (!(IFG2&UCA0TXIFG));              // USCI_A0 TX buffer ready?
  }
}

void transmitDataString(char data_mode, char addr[4],char rssi[3], char tx_msg_hs[MESSAGE_LENGTH] )
// Assemble the Access Point data string to be send to the host.
// data_mode:	 			= degCmode  1 = Degree Celsius    0 = Degree Fahrenheit
// addr[4]:					Node Adress in the format 00XY
// rssi[3]:					Received Signal Strength in the format XYZ
// msg[MESSAGE_LENGTH]:		Additional message
// "MESSAGE Length" is defined in virtual_com_cmds.h:
// e.g. #define MESSAGE_LENGTH 3
{
  char temp_string[] = {" XX.XC"};
  int temp = tx_msg_hs[0] + (tx_msg_hs[1]<<8);
  
//  LED_RED = 1;

  if(!(data_mode & degCMode))
  {
    temp = (((float)temp)*1.8)+320;
    temp_string[5] = 'F';
  }
  if( temp < 0 )
  {
    temp_string[0] = '-';
    temp = temp * -1;
  }
  else if( ((temp/1000)%10) != 0 )
  {
    temp_string[0] = '0'+((temp/1000)%10);
  }
  temp_string[4] = '0'+(temp%10);
  temp_string[2] = '0'+((temp/10)%10);
  temp_string[1] = '0'+((temp/100)%10);
  
  if(data_mode & verboseMode)
  {
  	//                         0 00000000011111111112222222222333333333344444444445555555
  	//                         0 12345678901234567890123456789012345678901234567890123456
    char output_verbose[] = {"\r\nNode:XXXX,Temp:-XX.XC,Battery:X.XV,Strength:XXX%,RE:no "};
	//                                 ^^^^      ^^^^^^         ^ ^           ^^^
    output_verbose[46] = rssi[2];
    output_verbose[47] = rssi[1];
    output_verbose[48] = rssi[0];
    
    output_verbose[17] = temp_string[0];
    output_verbose[18] = temp_string[1];
    output_verbose[19] = temp_string[2];
    output_verbose[20] = temp_string[3];
    output_verbose[21] = temp_string[4];
    output_verbose[22] = temp_string[5];
    
    output_verbose[32] = '0'+(tx_msg_hs[2]/10)%10;
    output_verbose[34] = '0'+(tx_msg_hs[2]%10);
    output_verbose[7] = addr[0];
    output_verbose[8] = addr[1];
    output_verbose[9] = addr[2];
    output_verbose[10] = addr[3];
    TXString(output_verbose, sizeof output_verbose );
  }
  else
  {
 	//                       0 000000000111111111122222
  	//                       0 123456789012345678901234
    char output_short[] = {"\r\n$ADDR,-XX.XC,V.C,RSI,N#"};
    //                           ^^^^ ^^^^^^ ^ ^ ^^^
    output_short[19] = rssi[2];
    output_short[20] = rssi[1];
    output_short[21] = rssi[0];
    
    
    output_short[8] = temp_string[0];
    output_short[9] = temp_string[1];
    output_short[10] = temp_string[2];
    output_short[11] = temp_string[3];
    output_short[12] = temp_string[4];
    output_short[13] = temp_string[5];
   
    output_short[15] = '0'+(tx_msg_hs[2]/10)%10;
    output_short[17] = '0'+(tx_msg_hs[2]%10);
    output_short[3] = addr[0];
    output_short[4] = addr[1];
    output_short[5] = addr[2];
    output_short[6] = addr[3];
    TXString(output_short, sizeof output_short );
  }
}

void transmitData(int addr, signed char rssi,  char tx_msg_hs[MESSAGE_LENGTH] )
// Assemble the End Device data string to be send to the host.
{
  char addrString[4];
  char rssiString[3];
  volatile signed int rssi_int;

  addrString[0] = '0';
  addrString[1] = '0';
  addrString[2] = '0'+(((addr+1)/10)%10);
  addrString[3] = '0'+((addr+1)%10);
  rssi_int = (signed int) rssi;
  rssi_int = rssi_int+128;
  rssi_int = (rssi_int*100)/256;
  rssiString[0] = '0'+(rssi_int%10);
  rssiString[1] = '0'+((rssi_int/10)%10);
  rssiString[2] = '0'+((rssi_int/100)%10);

  transmitDataString( degCMode, addrString, rssiString, tx_msg_hs );

}

/*------------------------------------------------------------------------------
* USCIA interrupt service routine
------------------------------------------------------------------------------*/
#pragma vector=USCIAB0RX_VECTOR
// char IO_Command[] = {"$CCC,TTTT,VVVVVVV#"}; // #Command, Target, Value%

__interrupt void USCI0RX_ISR(void)
{
  char rx = UCA0RXBUF;             // Read UCA0RXBUF (clears UCA0RXIFG)
  if (buffer_active_flag == 0)
  {
  	if ( rx == 'V' || rx == 'v' )
  	{
    	verboseMode = 1;
  	}
  	else if ( rx == 'M' || rx == 'm' )
  	{
    	verboseMode = 0;
  	}
  	else if ( rx == 'F' || rx == 'f' )
  	{
    	degCMode = 0;
  	}
  	else if ( rx == 'C' || rx == 'c' )
  	{
    	degCMode = 1;
  	}
  	else if ( rx == 'L' || rx == 'l' )
  	{
    	if (LED_RED == 0)
    	{ 
    		LED_RED=1;		// Turn on red LED on ED's
    	}
    	else
    	{
    		LED_RED=0;		// Turn off red LED on ED's
    	}
    			
  	}
  	else if ( rx == '$')
  	{
  		if (buffer_full_flag == 0)				// Is buffer full?
  		{
   			buffer_active_flag = 1;					// Start reading command into buffer
  			buffer_end_pointer = 0;
  			io_command[buffer_end_pointer++] = rx;  // Read in first Character ("$")
  			//P1OUT |= 0x01;   						// Turn on red LED
  		}
  		else
  		{
  		 	buffer_error_flag = 2; 			// Error #2: Buffer is not empty
  		}
  	}
  }
  else
  {
  	if (buffer_error_flag == 0)		// Only write to buffer if there is no error
  	{
  		if ( rx == '#' )
  		{
  			io_command[buffer_end_pointer++] = rx;		// Read last character into buffer and increment pointer
  			buffer_active_flag = 0;						// Finished reading command into buffer
  			buffer_full_flag = 1;						// There is data waiting in the buffer
  			//P1OUT &= ~0x01;   							// Turn off red LED
  		}
  		else
  		{
  			io_command[buffer_end_pointer++] = rx;		// Read character into buffer and increment pointer
  		}
    	if (buffer_end_pointer>19)
    	{
    		buffer_active_flag = 0;		// Clear Buffer Flag to avoid Buffer overrun
    		buffer_full_flag = 1;		// There is data waiting in the buffer
    		buffer_error_flag = 1;		// Error #1: Buffer Overrun
    	}
  	}
  }	
  IFG2 &= ~UCA0RXIFG;                          // Clear USCI_A0 RX interrupt Flag (we should not need that)
}

void parseBuffer(void)
// Parse Buffer
//                      01234567890123456789
//                      00000000001111111111
//char io_command[] = {"$CCCC,TTTT,VVVVVVVV# "}; // $Command, Target, Value#
//                      00000000001111111111
//                      01234567890123456789
//char cmd[] = 	      {"CCCC"};
//                      0123
//char target[] =     {"TTTT"};
//                      0123
//char value[] =      {"VVVVVVVV"};
//                      01234567
{
cmd[0] = io_command[1];
cmd[1] = io_command[2];
cmd[2] = io_command[3];
cmd[3] = io_command[4];

target[0] = io_command[6];
target[1] = io_command[7];
target[2] = io_command[8];
target[3] = io_command[9];

value[0] = io_command[11];
value[1] = io_command[12];
value[2] = io_command[13];
value[3] = io_command[14];
value[4] = io_command[15];
value[5] = io_command[16];
value[6] = io_command[17];
value[7] = io_command[18];
}

static char LookUpTable(void)
// This function compares the received command "cmd" with string values in the lookup table "cmd_table".
// It either returns the location of a string match or the value 99 as indicator that no match was found.
{
  char i;
  for (i=0; i<NOC; ++i)
  {
  	if (strcmp(cmd,cmd_table[i])==0)
  	{
  		return i;	// Command Value
  	}
  }
   return 99;		// Error - not a command
}
