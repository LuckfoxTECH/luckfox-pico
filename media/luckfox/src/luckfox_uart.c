/*****************************************************************************
* | File        :   luckfox_uart.c
* | Author      :   luckfox team
* | Function    :   Read and write /dev/UART,  hardware UART
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2023-08-01
* | Info        :   Basic version
*
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "luckfox_uart.h"


#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

HARDWARE_UART hardware_UART;
struct termios UART_Set;

/******************************************************************************
function:   Serial port initialization
parameter:
    UART_device : Device name
Info:
    /dev/ttyTHS*
    The default baud rate is 9600, 8-bit data, 1 stop bit, no parity
******************************************************************************/
void luckfox_uart_begin(char *UART_device)
{
    //device
    if((hardware_UART.fd = open(UART_device, O_RDWR | O_NOCTTY)) < 0)  { //打开UART 
        perror("Failed to open UART device.\n");  
        exit(1); 
    } else {
        LUCKFOX_UART_DEBUG("open : %s\r\n", UART_device);
    }
    luckfox_uart_setBaudrate(115200);
    luckfox_uart_Set(8, 1, 'N');
}

/******************************************************************************
function:   Serial device End
parameter:
Info:
******************************************************************************/
void luckfox_uart_end(void)
{
    if (close(hardware_UART.fd) != 0){
        LUCKFOX_UART_DEBUG("Failed to close UART device\r\n");
        perror("Failed to close UART device.\n");  
    }
}

/******************************************************************************
function:   Set the serial port baud rate
parameter:
    Baudrate : Serial port baud rate
               B50          50
               B75          75
               B110         110
               B134         134
               B150         150
               B200         200
               B300         300
               B600         600
               B1200        1200
               B1800        1800
               B2400        2400
               B4800        4800
               B9600        9600
               B19200       19200
               B38400       38400
               B57600       57600
               B115200      115200
               B230400      230400
Info:
******************************************************************************/
void luckfox_uart_setBaudrate(uint32_t Baudrate)
{
    uint16_t err;
    uint32_t baud;
    tcflush(hardware_UART.fd, TCIOFLUSH);//清空数据缓冲区
    
    switch(Baudrate)        //Set the number of data bits
    {
        case B230400:
        case 230400:    baud = B230400; break;
        case B115200:   
        case 115200:    baud = B115200; break;
        case B57600:   
        case 57600:     baud = B57600; break;
        case B38400:   
        case 38400:     baud = B38400; break;
        case B19200:   
        case 19200:     baud = B19200; break;
        case B9600:   
        case 9600:  baud = B9600; break;
        case B4800:   
        case 4800:  baud = B4800; break;
        case B2400:   
        case 2400:  baud = B2400; break;
        case B1800:   
        case 1800:  baud = B1800; break;
        case B1200:   
        case 1200:  baud = B1200; break;
        case B600:   
        case 600:   baud = B600; break;
        case B300:   
        case 300:   baud = B300; break;
        case B200:   
        case 200:   baud = B200; break;
        case B150:   
        case 150:   baud = B150; break;
        case B134:   
        case 134:   baud = B134; break;
        case B110:   
        case 110:   baud = B110; break;
        case B75:   
        case 75:    baud = B75; break;
        case B50:   
        case 50:    baud = B50; break;
        default:    baud = B9600;break;

    }
    LUCKFOX_UART_DEBUG("Baud rate setting\r\n");
 	if(cfsetispeed(&UART_Set, baud) != 0){
        LUCKFOX_UART_DEBUG("Baud rate setting failed 1\r\n");
    }
	if(cfsetospeed(&UART_Set, baud) != 0){
        LUCKFOX_UART_DEBUG("Baud rate setting failed 2\r\n");
        
    }
    err = tcsetattr(hardware_UART.fd, TCSANOW, &UART_Set);
    if(err != 0){
        perror("tcsetattr fd");
        LUCKFOX_UART_DEBUG("Setting the terminal baud rate failed\r\n");
    }
}

/******************************************************************************
function: Serial port sends one byte of data
parameter:
    buf :   Sent data
Info:
******************************************************************************/
uint8_t luckfox_uart_writeByte(uint8_t buf)
{
    uint8_t sbuf[1] = {0};
    sbuf[0] = buf;
    write(hardware_UART.fd, sbuf, 1);
    return 0;
}

/******************************************************************************
function: Serial port sends arbitrary length data
parameter:
    buf :   Sent Data buffer
    len :   Number of bytes sent
Info:
******************************************************************************/
uint8_t luckfox_uart_write(const char * buf, uint32_t len)
{
    write(hardware_UART.fd, buf, len); 
    return 0;
}

/******************************************************************************
function: The serial port reads a byte
parameter:
Info: Return read data
******************************************************************************/
uint8_t luckfox_uart_readByte(void)
{
    uint8_t buf[1] = {0};
    read(hardware_UART.fd, buf, 1);
    return buf[0];
}

/******************************************************************************
function: Serial port reads arbitrary byte length data
parameter:
    buf :   Read Data buffer
    len :   Number of bytes Read
Info:
******************************************************************************/
uint8_t luckfox_uart_read(char* buf, uint32_t len)
{
    read(hardware_UART.fd, buf, len);
    return 0;
}

/******************************************************************************
function: Set serial port parameters
parameter:
    databits :   Data bit
    stopbits :   Stop bit
    parity   :   Parity bit
Info:
******************************************************************************/
int luckfox_uart_Set(int databits, int stopbits, int parity)
{
    if(tcgetattr(hardware_UART.fd, &UART_Set) != 0)
    {
        perror("tcgetattr fd");
        LUCKFOX_UART_DEBUG("Failed to get terminal parameters\r\n");
        return 0;
    }
   UART_Set.c_cflag |= (CLOCAL | CREAD);        //Generally set flag

    switch(databits)        //Set the number of data bits
    {
    case 5:
        UART_Set.c_cflag &= ~CSIZE;
        UART_Set.c_cflag |= CS5;
        break;
    case 6:
        UART_Set.c_cflag &= ~CSIZE;
        UART_Set.c_cflag |= CS6;
        break;
    case 7:
        UART_Set.c_cflag &= ~CSIZE;
        UART_Set.c_cflag |= CS7;
        break;
    case 8:
        UART_Set.c_cflag &= ~CSIZE;
        UART_Set.c_cflag |= CS8;
        break;
    default:
        fprintf(stderr, "Unsupported data size.\n");
        return 0;
    }

    switch(parity)            //Set check digit
    {
    case 'n':
    case 'N':
        UART_Set.c_cflag &= ~PARENB;        //Clear check digit
        UART_Set.c_iflag &= ~INPCK;         //enable parity checking
        break;
    case 'o':
    case 'O':
        UART_Set.c_cflag |= PARENB;         //enable parity
        UART_Set.c_cflag |= PARODD;         //Odd parity
        UART_Set.c_iflag |= INPCK;          //disable parity checking
        break;
    case 'e':     
    case 'E':        
        UART_Set.c_cflag |= PARENB;         //enable parity        
        UART_Set.c_cflag &= ~PARODD;        //Even parity         
        UART_Set.c_iflag |= INPCK;          //disable pairty checking        
        break;     
    case 's':    
    case 'S':         
        UART_Set.c_cflag &= ~PARENB;        //Clear check digit         
        UART_Set.c_cflag &= ~CSTOPB;
        UART_Set.c_iflag |= INPCK;          //disable pairty checking        
        break;    
    default:         
        fprintf(stderr, "Unsupported parity.\n");        
        return 0;        
    }
    switch(stopbits)        //Set stop bit  1   2
    {     
        case 1:         
            UART_Set.c_cflag &= ~CSTOPB;        
            break;     
        case 2:        
            UART_Set.c_cflag |= CSTOPB;    
            break;   
        default:     
            fprintf(stderr, "Unsupported stopbits.\n");         
            return 0;     
    }    
    UART_Set.c_cflag |= (CLOCAL | CREAD);
    UART_Set.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    UART_Set.c_oflag &= ~OPOST;
    UART_Set.c_oflag &= ~(ONLCR | OCRNL);
    UART_Set.c_iflag &= ~(ICRNL | INLCR);    
    UART_Set.c_iflag &= ~(IXON | IXOFF | IXANY);
    tcflush(hardware_UART.fd, TCIFLUSH);
    UART_Set.c_cc[VTIME] = 150;        //Set timeout
    UART_Set.c_cc[VMIN] = 1;        //
    if(tcsetattr(hardware_UART.fd, TCSANOW, &UART_Set) != 0)   //Update the UART_Set and do it now  
    {
        perror("tcsetattr fd");
        LUCKFOX_UART_DEBUG("Setting terminal parameters failed\r\n");
        return 0;    
    }   
    LUCKFOX_UART_DEBUG("Set terminal parameters successfully\r\n");
    return 1;
}








