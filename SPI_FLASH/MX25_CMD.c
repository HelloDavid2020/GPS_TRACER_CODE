/*
 * COPYRIGHT (c) 2010 MACRONIX INTERNATIONAL CO., LTD
 * SPI Flash Low Level Driver (LLD) Sample Code
 *
 * SPI interface command set
 *
 * $Id: MX25_CMD.c,v 1.5 2010/06/02 01:33:35 benhuang Exp $
 */



#include "MX25_CMD.h"
uint32 flash_id = 0;

/*
 --Common functions
 */

/*
 * Function:       Wait_Flash_WarmUp
 * Arguments:      None.
 * Description:    Wait some time until flash read / write enable.
 * Return Message: None.
 */
void Wait_Flash_WarmUp()
{
    uint32 time_cnt = FlashFullAccessTime;
    while( time_cnt > 0 )
    {
        time_cnt--;
    }
}

/*
 * Function:       Initial_Spi
 * Arguments:      None
 * Description:    Initial spi flash state and wait flash warm-up
 *                 (enable read/write).
 * Return Message: None
 */
//void Initial_Spi()
//{

//#ifdef GPIO_SPI
//    WPn = 1;        // Write potected initial high
//    SI = 0;         // Flash input data
//    SCLK = 1;       // Flash input clock
//    CSn = 1;        // Chip Select
//#endif

//    // Wait flash warm-up
//    Wait_Flash_WarmUp();
//}

/*
 * Function:       CS_Low, CS_High
 * Arguments:      None.
 * Description:    Chip select go low / high.
 * Return Message: None.
 */
//void CS_Low()
//{
//#ifdef GPIO_SPI
//    CSn = 0;
//#else
//    //--- insert your chip select code here. ---//
//#endif
//}

//void CS_High()
//{
//#ifdef GPIO_SPI
//    CSn = 1;
//#else
//    //--- insert your chip select code here. ---//
//#endif
//}

/*
 * Function:       InsertDummyCycle
 * Arguments:      dummy_cycle, number of dummy clock cycle
 * Description:    Insert dummy cycle of SCLK
 * Return Message: None.
 */
void InsertDummyCycle( uint8 dummy_cycle )
{
#ifdef GPIO_SPI
    uint8 i;
    for( i=0; i < dummy_cycle; i += 8 )
    {
        SendByte(0, 0);
    }
#else
    //--- insert your code here. ---//
#endif
}

/*
 * Function:       SendByte
 * Arguments:      byte_value, data transfer to flash
 *                 transfer_type, select different type of I/O mode.
 *                 Seven mode:
 *                 SIO, single IO
 *                 DIO, dual IO
 *                 QIO, quad IO
 *                 PIO, parallel
 *                 DTSIO, double transfer rate SIO
 *                 DTDIO, double transfer rate DIO
 *                 DTQIO, double transfer rate QIO
 * Description:    Send one byte data to flash
 * Return Message: None.
 */
//void SendByte( uint8 byte_value, uint8 transfer_type )
//{
//#ifdef GPIO_SPI
//    uint16 i;
//    uint8 cycle_cnt;

//    switch( transfer_type )
//    {
//#ifdef SIO
//    case SIO: // single I/O
//        cycle_cnt = 8;
//        for( i= 0; i < cycle_cnt; i++ )
//        {
//            if ( (byte_value & IO_MASK) == 0x80 ){
//                SI = 1;
//            }
//            else{
//                SI = 0;
//            }
//            SCLK = 0;
//            byte_value = byte_value << 1;
//            SCLK = 1;
//        }
//        break;
//#endif
//#ifdef DIO
//    case DIO: // dual I/O
//        cycle_cnt = 4;
//        for( i= 0; i < cycle_cnt; i++ )
//        {
//            SCLK = 0;
//            P1 = (( byte_value & 0xc0 ) >> 2) | (P1 & 0xcf);
//            byte_value = byte_value << 2;
//            SCLK = 1;
//        }
//        break;
//#endif
//#ifdef QIO
//    case QIO: // quad I/O
//        cycle_cnt = 2;
//        for( i= 0; i < cycle_cnt; i++ )
//        {
//            P1 = (byte_value & 0xf0);  // CS# and SCLK must be zero at this phase
//            byte_value = byte_value << 4;
//            SCLK = 1;
//        }
//        break;
//#endif
//#ifdef PIO
//    case PIO: //  Parallel I/O
//        SCLK = 0; 
//        PO7 = ( (byte_value & IO_MASK)  == IO_MASK )?1:0;
//        PO6 = ( (byte_value & 0x40)  == 0x40 )?1:0;
//        P3 = (byte_value & 0x3f) | (P3 & 0xc0);
//        SCLK = 1;
//        break;
//#endif
//#ifdef DTSIO
//    case DTSIO: //  Double transfer rate single I/O
//        cycle_cnt = 4;
//         for( i= 0; i < cycle_cnt; i++ )
//        {
//            SCLK = 0;
//            SI = ( (byte_value & IO_MASK) == IO_MASK )?1:0;
//            byte_value = byte_value << 1;

//            SCLK = 1;
//            SI = ( (byte_value & IO_MASK) == IO_MASK )?1:0;
//            byte_value = byte_value << 1;
//        }
//        break;
//#endif
//#ifdef DTDIO
//    case DTDIO: //  Double transfer rate dual I/O
//        cycle_cnt = 2;
//        for( i= 0; i < cycle_cnt; i++ )
//        {
//            SCLK = 0;
//            P1 = (( byte_value & 0xc0 ) >> 2) | (P1 & 0xcf);
//            byte_value = byte_value << 2;
//            SCLK = 1;
//            P1 = (( byte_value & 0xc0 ) >> 2) | (P1 & 0xcf);
//            byte_value = byte_value << 2;
//        }
//        break;
//#endif
//#ifdef DTQIO
//    case DTQIO: //  Double transfer rate quad I/O
//        SCLK = 0;
//        P1 = (byte_value & 0xf0);
//        SCLK = 1;
//        byte_value = byte_value << 4;
//        P1 = (byte_value & 0xf0);  // CS# and SCLK must be zero at this phase
//        break;
//#endif
//    default:
//        break;
//    }
//#else
//    switch( transfer_type )
//    {
//#ifdef SIO
//    case SIO: // Single I/O
//        //--- insert your code here for single IO transfer. ---//
//        break;
//#endif
//#ifdef DIO
//    case DIO: // Dual I/O
//        //--- insert your code here for dual IO transfer. ---//
//        break;
//#endif
//#ifdef QIO
//    case QIO: // Quad I/O
//        //--- insert your code here for quad IO transfer. ---//
//        break;
//#endif
//#ifdef PIO
//    case PIO: // Parallel I/O
//        //--- insert your code here for parallel IO transfer. ---//
//        break;
//#endif
//#ifdef DTSIO
//    case DTSIO: // Double transfer rate Single I/O
//        //--- insert your code here for DT single IO transfer. ---//
//        break;
//#endif
//#ifdef DTDIO
//    case DTDIO: // Double transfer rate Dual I/O
//        //--- insert your code here for DT dual IO transfer. ---//
//        break;
//#endif
//#ifdef DTQIO
//    case DTQIO: // Double transfer rate Quad I/O
//        //--- insert your code here for DT quad IO transfer. ---//
//        break;
//#endif
//    default:
//        break;
//    }
//#endif  /* End of GPIO_SPI */
//}
/*
 * Function:       GetByte
 * Arguments:      byte_value, data receive from flash
 *                 transfer_type, select different type of I/O mode.
 *                 Seven mode:
 *                 SIO, single IO
 *                 DIO, dual IO
 *                 QIO, quad IO
 *                 PIO, parallel IO
 *                 DTSIO, double transfer rate SIO
 *                 DTDIO, double transfer rate DIO
 *                 DTQIO, double transfer rate QIO
 * Description:    Get one byte data to flash
 * Return Message: 8 bit data
 */
//uint8 GetByte( uint8 transfer_type )
//{
//#ifdef GPIO_SPI
//    uint16 i;
//    uint8 cycle_cnt;
//    uint8 data_buf;
//    data_buf = 0;
//    cycle_cnt = 8 >> transfer_type;

//    switch( transfer_type )
//    {
//#ifdef SIO
//    case SIO: // single I/O
//        // Set VIP 8051 GPIO as input ( need pull to high )
//        SO = 1;
//        // End VIP 8051 GPIO

//        for( i= 0; i < cycle_cnt; i++ )
//        {
//            SCLK = 0;
//            if ( SO == 1 ){
//                data_buf = (data_buf | (0x80 >> i));
//            }
//            SCLK = 1;
//        }
//        break;
//#endif
//#ifdef DIO
//    case DIO: // dual I/O
//        // Set VIP 8051 GPIO as input ( need pull to high )
//        SIO0 = 1;
//        SIO1 = 1;
//        // End VIP 8051 GPIO

//        for( i= 0; i < cycle_cnt; i++ )
//        {
//            SCLK = 0;
//            data_buf = data_buf << 2;
//            data_buf = ( data_buf | ((P1 & 0x30) >> 4 ) );
//            SCLK = 1;
//        }
//        break;
//#endif
//#ifdef QIO
//    case QIO: // quad I/O
//        // Set VIP 8051 GPIO as input ( need pull to high )
//        SIO0 = 1;
//        SIO1 = 1;
//        SIO2 = 1;
//        SIO3 = 1;
//        // End VIP 8051 GPIO
//        SCLK = 0;
//        data_buf = P1 & 0xf0;
//        SCLK = 1;
//        SCLK = 0;
//        data_buf = ((P1 & 0xf0)>> 4)| data_buf;
//        SCLK = 1;
//        break;
//#endif
//#ifdef PIO
//    case PIO: //  Parallel I/O
//        // Set VIP 8051 GPIO as input ( need pull to high )
//        PO7 = 1;
//        PO6 = 1;
//        PO5 = 1;
//        PO4 = 1;
//        PO3 = 1;
//        PO2 = 1;
//        PO1 = 1;
//        PO0 = 1;
//        // End VIP 8051 GPIO
//        SCLK = 0;
//        data_buf = (( P1 & 0x20 )<< 2) | ((P1 & 0x02 ) << 5) | (P3 & 0x3f );
//        SCLK = 1;
//        break;
//#endif
//#ifdef DTSIO
//    case DTSIO: //  Double transfer rate Single I/O
//        // Set VIP 8051 GPIO as input ( need pull to high )
//        SO = 1;
//        // End VIP 8051 GPIO
//        cycle_cnt = 4;
//        for( i= 0; i < cycle_cnt; i++ )
//        {
//            SCLK = 0;
//            if ( SO == 1 ){
//                data_buf = (data_buf | ( 0x80 >> (i*2) ));
//            }
//            SCLK = 1;
//            if ( SO == 1 ){
//                data_buf = (data_buf | (0x80 >> ((i*2) + 1) ));
//            }
//        }
//        break;
//#endif
//#ifdef DTDIO
//    case DTDIO: //  Double transfer rate Dual I/O
//        // Set VIP 8051 GPIO as input ( need pull to high )
//        SIO0 = 1;
//        SIO1 = 1;
//        // End VIP 8051 GPIO
//        cycle_cnt = 2;
//        for( i= 0; i < cycle_cnt; i++ )
//        {
//            SCLK = 0;
//            data_buf = data_buf << 2;
//            data_buf = ( data_buf  | ( (P1 & 0x30) >> 4 ) );
//            SCLK = 1;
//            data_buf = data_buf << 2;
//            data_buf = ( data_buf  | ( (P1 & 0x30) >> 4 ) );
//        }
//        break;
//#endif
//#ifdef DTQIO
//    case DTQIO: //  DTR qual I/O
//        // Set VIP 8051 GPIO as input ( need pull to high )
//        SIO0 = 1;
//        SIO1 = 1;
//        SIO2 = 1;
//        SIO3 = 1;
//        // End VIP 8051 GPIO
//        SCLK = 0;
//        data_buf = P1 & 0xf0;
//        SCLK = 1;
//        data_buf = ( (P1 & 0xf0) >> 4 )| data_buf;
//        break;
//#endif
//    default:
//        break;

//    }
//#else
//    switch( transfer_type )
//    {
//#ifdef SIO
//    case SIO: // Single I/O
//        //--- insert your code here for single IO receive. ---//
//        break;
//#endif
//#ifdef DIO
//    case DIO: // Dual I/O
//        //--- insert your code here for dual IO receive. ---//
//        break;
//#endif
//#ifdef QIO
//    case QIO: // Quad I/O
//        //--- insert your code here for qual IO receive. ---//
//        break;
//#endif
//#ifdef PIO
//    case PIO: // Parallel I/O
//        //--- insert your code here for parallel IO receive. ---//
//        break;
//#endif
//#ifdef DTSIO
//    case DTSIO: // Double transfer rate Single I/O
//        //--- insert your code here for DT single IO receive. ---//
//        break;
//#endif
//#ifdef DTDIO
//    case DTDIO: // Double transfer rate Dual I/O
//        //--- insert your code here for DT dual IO receive. ---//
//        break;
//#endif
//#ifdef DTQIO
//    case DTQIO: // Double transfer rate Qual I/O
//        //--- insert your code here for DT quad IO receive. ---//
//#endif
//    default:
//        break;
//    }
//#endif  /* End of GPIO_SPI */
//    return data_buf;
//}

/*
 * Function:       WaitFlashReady
 * Arguments:      ExpectTime, expected time-out value of flash operations.
 *                 No use at non-synchronous IO mode.
 * Description:    Synchronous IO:
 *                 If flash is ready return TRUE.
 *                 If flash is time-out return FALSE.
 *                 Non-synchronous IO:
 *                 Always return TRUE
 * Return Message: TRUE, FALSE
 */
BOOL WaitFlashReady( uint32 ExpectTime )
{
#ifndef NON_SYNCHRONOUS_IO
    uint32 temp = 0;
    while( IsFlashBusy() )
    {
        if( temp > ExpectTime )
        {
            return FALSE;
        }
        temp = temp + 1;
    }
       return TRUE;
#else
    return TRUE;
#endif
}

/*
 * Function:       WaitRYBYReady
 * Arguments:      ExpectTime, expected time-out value of flash operations.
 *                 No use at non-synchronous IO mode.
 * Description:    Synchronous IO:
 *                 If flash is ready return TRUE.
 *                 If flash is time-out return FALSE.
 *                 Non-synchronous IO:
 *                 Always return TRUE
 * Return Message: TRUE, FALSE
 */
BOOL WaitRYBYReady( uint32 ExpectTime )
{
#ifndef NON_SYNCHRONOUS_IO
    uint32 temp = 0;
#ifdef GPIO_SPI
    //while( SO == 0 )
	return TRUE;
#else
    // Insert your code for waiting RYBY (SO) pin ready
#endif
    {
        if( temp > ExpectTime )
        {
            return FALSE;
        }
        temp = temp + 1;
    }
    return TRUE;

#else
    return TRUE;
#endif
}

/*
 * Function:       IsFlashBusy
 * Arguments:      None.
 * Description:    Check status register WIP bit.
 *                 If  WIP bit = 1: return TRUE ( Busy )
 *                             = 0: return FALSE ( Ready ).
 * Return Message: TRUE, FALSE
 */
BOOL IsFlashBusy( void )
{
    uint8  gDataBuffer;

    CMD_RDSR( &gDataBuffer );
    if( (gDataBuffer & FLASH_WIP_MASK)  == FLASH_WIP_MASK )
        return TRUE;
    else
        return FALSE;
}

/*
 * Function:       IsFlashQIO
 * Arguments:      None.
 * Description:    If flash QE bit = 1: return TRUE
 *                                 = 0: return FALSE.
 * Return Message: TRUE, FALSE
 */
BOOL IsFlashQIO( void )
{
    uint8  gDataBuffer;
    CMD_RDSR( &gDataBuffer );
    if( (gDataBuffer & FLASH_QE_MASK) == FLASH_QE_MASK )
        return TRUE;
    else
        return FALSE;
}
/*
 * Function:       IsFlash4Byte
 * Arguments:      None
 * Description:    Check flash address is 3-byte or 4-byte.
 *                 If flash 4BYTE bit = 1: return TRUE
 *                                    = 0: return FALSE.
 * Return Message: TRUE, FALSE
 */
BOOL IsFlash4Byte( void )
{
#ifdef FLASH_CMD_RDSCUR
	#ifdef FLASH_4BYTE_ONLY
		return TRUE;
	#else
	    uint8  gDataBuffer;
    	CMD_RDSCUR( &gDataBuffer );
	    if( (gDataBuffer & FLASH_4BYTE_MASK) == FLASH_4BYTE_MASK )
    	    return TRUE;
	    else
	        return FALSE;
	#endif
#else
    return FALSE;
#endif
//	return FALSE;
}
/*
 * Function:       SendFlashAddr
 * Arguments:      flash_address, 32 bit flash memory address
 *                 io_mode, I/O mode to transfer address
 *                 addr_4byte_mode, 
 * Description:    Send flash address with 3-byte or 4-byte mode.
 * Return Message: None
 */
void SendFlashAddr( uint32 flash_address, uint8 io_mode, BOOL addr_4byte_mode )
{
    /* Check flash is 3-byte or 4-byte mode.
       4-byte mode: Send 4-byte address (A31-A0)
       3-byte mode: Send 3-byte address (A23-A0) */
    if( addr_4byte_mode == TRUE ){
        SendByte( (flash_address >> 24), io_mode ); // A31-A24
    }
    /* A23-A0 */
    SendByte( (flash_address >> 16), io_mode );
    SendByte( (flash_address >> 8), io_mode );
    SendByte( (flash_address), io_mode );
}

/*
 * ID Command
 */

/*
 * Function:       CMD_RDID
 * Arguments:      Identification, 32 bit buffer to store id
 * Description:    The RDID instruction is to read the manufacturer ID
 *                 of 1-byte and followed by Device ID of 2-byte.
 * Return Message: FlashOperationSuccess
 */
ReturnMsg CMD_RDID( uint32 *Identification )
{
    uint32 temp;
    uint8  gDataBuffer[3];

    // Chip select go low to start a flash command
    CS_Low();

    // Send command
    SendByte( FLASH_CMD_RDID, SIO );

    // Get manufacturer identification, device identification
    gDataBuffer[0] = GetByte( SIO );
    gDataBuffer[1] = GetByte( SIO );
    gDataBuffer[2] = GetByte( SIO );

    // Chip select go high to end a command
    CS_High();

    // Store identification
    temp =  gDataBuffer[0];
    temp =  (temp << 8) | gDataBuffer[1];
    *Identification =  (temp << 8) | gDataBuffer[2];

    return FlashOperationSuccess;
}

/*
 * Function:       CMD_RES
 * Arguments:      ElectricIdentification, 8 bit buffer to store electric id
 * Description:    The RES instruction is to read the Device
 *                 electric identification of 1-byte.
 * Return Message: FlashOperationSuccess
 */
ReturnMsg CMD_RES( uint8 *ElectricIdentification )
{

    // Chip select go low to start a flash command
    CS_Low();

    // Send flash command and insert dummy cycle
    SendByte( FLASH_CMD_RES, SIO );
    InsertDummyCycle( 24 );

    // Get electric identification
    *ElectricIdentification = GetByte( SIO );

    // Chip select go high to end a flash command
    CS_High();

    return FlashOperationSuccess;
}

/*
 * Function:       CMD_REMS
 * Arguments:      REMS_Identification, 16 bit buffer to store id
 *                 fsptr, pointer of flash status structure
 * Description:    The REMS instruction is to read the Device
 *                 manufacturer ID and electric ID of 1-byte.
 * Return Message: FlashOperationSuccess
 */
ReturnMsg CMD_REMS( uint16 *REMS_Identification, FlashStatus *fsptr )
{
    uint8  gDataBuffer[2];

    // Chip select go low to start a flash command
    CS_Low();

    // Send flash command and insert dummy cycle ( if need )
    // ArrangeOpt = 0x00 will output the manufacturer's ID first
    //            = 0x01 will output electric ID first
    SendByte( FLASH_CMD_REMS, SIO );
    InsertDummyCycle( 16 );
    SendByte( fsptr->ArrangeOpt, SIO );

    // Get ID
    gDataBuffer[0] = GetByte( SIO );
    gDataBuffer[1] = GetByte( SIO );

    // Store identification informaion
    *REMS_Identification = (gDataBuffer[0] << 8) | gDataBuffer[1];

    // Chip select go high to end a flash command
    CS_High();

    return FlashOperationSuccess;
}

/*
 * Function:       CMD_REMS2
 * Arguments:      REMS_Identification, 16 bit buffer to store id
 *                 fsptr, pointer of flash status structure
 * Description:    The REMS2 instruction is to read the Device
 *                 manufacturer ID and electric ID of 1-byte.
 * Return Message: FlashOperationSuccess
 */
ReturnMsg CMD_REMS2( uint16 *REMS_Identification, FlashStatus *fsptr )
{
    uint8  gDataBuffer[2];

    // Chip select go low to start a flash command
    CS_Low();

    // Send flash command and insert dummy cycle ( if need )
    // ArrangeOpt = 0x00 will output the manufacturer's ID first
    //            = 0x01 will output electric ID first
    SendByte( FLASH_CMD_REMS2, SIO );
    InsertDummyCycle( 16 );
    SendByte( fsptr->ArrangeOpt, SIO );

    // Get ID
    gDataBuffer[0] = GetByte( SIO );
    gDataBuffer[1] = GetByte( SIO );

    // Store identification informaion
    *REMS_Identification = (gDataBuffer[0] << 8) | gDataBuffer[1];

    // Chip select go high to end a flash command
    CS_High();

    return FlashOperationSuccess;
}
/*
 * Function:       CMD_REMS4
 * Arguments:      REMS_Identification, 16 bit buffer to store id
 *                 fsptr, pointer of flash status structure
 * Description:    The REMS4 instruction is to read the Device
 *                 manufacturer ID and electric ID of 1-byte.
 * Return Message: FlashOperationSuccess
 */
ReturnMsg CMD_REMS4( uint16 *REMS_Identification, FlashStatus *fsptr )
{
    uint8  gDataBuffer[2];

    // Chip select go low to start a flash command
    CS_Low();

    // Send flash command and insert dummy cycle ( if need )
    // ArrangeOpt = 0x00 will output the manufacturer's ID first
    //            = 0x01 will output electric ID first
    SendByte( FLASH_CMD_REMS4, SIO );
    InsertDummyCycle( 16 );
    SendByte( fsptr->ArrangeOpt, SIO );

    // Get ID
    gDataBuffer[0] = GetByte( SIO );
    gDataBuffer[1] = GetByte( SIO );

    // Store identification informaion
    *REMS_Identification = (gDataBuffer[0] << 8) | gDataBuffer[1];

    // Chip select go high to end a flash command
    CS_High();

    return FlashOperationSuccess;
}


/*
 * Register  Command
 */

/*
 * Function:       CMD_RDSR
 * Arguments:      StatusReg, 8 bit buffer to store status register value
 * Description:    The RDSR instruction is for reading Status Register Bits.
 * Return Message: FlashOperationSuccess
 */
ReturnMsg CMD_RDSR( uint8 *StatusReg )
{
    uint8  gDataBuffer;

    // Chip select go low to start a flash command
    CS_Low();

    // Send command
    SendByte( FLASH_CMD_RDSR, SIO );
    gDataBuffer = GetByte( SIO );

    // Chip select go high to end a flash command
    CS_High();

    *StatusReg = gDataBuffer;

    return FlashOperationSuccess;
}

/*
 * Function:       CMD_WRSR
 * Arguments:      UpdateValue, 8 bit status register value to updata
 * Description:    The WRSR instruction is for changing the values of
 *                 Status Register Bits
 * Return Message: FlashIsBusy, FlashTimeOut, FlashOperationSuccess
 */
ReturnMsg CMD_WRSR( uint8 UpdateValue )
{

    // Check flash is busy or not
    if( IsFlashBusy() )    return FlashIsBusy;

    // Setting Write Enable Latch bit
    CMD_WREN();

    // Chip select go low to start a flash command
    CS_Low();

    // Send command and update value
    SendByte( FLASH_CMD_WRSR, SIO );
    SendByte( UpdateValue, SIO );

    // Chip select go high to end a flash command
    CS_High();


    if( WaitFlashReady( WriteStatusRegCycleTime ) )
        return FlashOperationSuccess;
    else
        return FlashTimeOut;

}

/*
 * Function:       CMD_RDSCUR
 * Arguments:      SecurityReg, 8 bit buffer to store security register value
 * Description:    The RDSCUR instruction is for reading the value of
 *                 Security Register bits.
 * Return Message: FlashOperationSuccess
 */
ReturnMsg CMD_RDSCUR( uint8 *SecurityReg )
{
    uint8  gDataBuffer;

    // Chip select go low to start a flash command
    CS_Low();

    //Send command
    SendByte( FLASH_CMD_RDSCUR, SIO );
    gDataBuffer = GetByte( SIO );

    // Chip select go high to end a flash command
    CS_High();

    *SecurityReg = gDataBuffer;

    return FlashOperationSuccess;

}

/*
 * Function:       CMD_WRSCUR
 * Arguments:      None.
 * Description:    The WRSCUR instruction is for changing the values of
 *                 Security Register Bits.
 * Return Message: FlashIsBusy, FlashOperationSuccess, FlashWriteRegFailed,
 *                 FlashTimeOut
 */
ReturnMsg CMD_WRSCUR( void )
{
    uint8  gDataBuffer;

    // Check flash is busy or not
    if( IsFlashBusy() )    return FlashIsBusy;

    // Setting Write Enable Latch bit
    CMD_WREN();

    // Chip select go low to start a flash command
    CS_Low();

    // Write WRSCUR command
    SendByte( FLASH_CMD_WRSCUR, SIO );

    // Chip select go high to end a flash command
    CS_High();

    if( WaitFlashReady( WriteSecuRegCycleTime ) ){

        CMD_RDSCUR( &gDataBuffer );

        // Check security register LDSO bit
        if( (gDataBuffer & FLASH_LDSO_MASK) == FLASH_LDSO_MASK )
                return FlashOperationSuccess;
        else
                return FlashWriteRegFailed;
    }
    else
        return FlashTimeOut;

}

/*
 * Function:       CMD_CLSR
 * Arguments:      None.
 * Description:    The CLSR instruction is for clearing security register
 *                 bit[5], bit[6].
 * Return Message: FlashOperationSuccess
 */
ReturnMsg CMD_CLSR( void )
{
    // Chip select go low to start a flash command
    CS_Low();

    // Write command
    SendByte( FLASH_CMD_CLSR, SIO );

    // Chip select go high to end a flash command
    CS_High();

    return FlashOperationSuccess;
}


/*
 * Read Command
 */

/*
 * Function:       CMD_READ
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 * Description:    The READ instruction is for reading data out.
 * Return Message: FlashAddressInvalid, FlashOperationSuccess
 */
ReturnMsg CMD_READ( uint32 flash_address, uint8 *target_address, uint32 byte_length )
{
    uint32 index;
    uint8  addr_4byte_mode;

    // Check flash address
    if( flash_address > FlashSize ) return FlashAddressInvalid;

    // Check 3-byte or 4-byte mode
    if( IsFlash4Byte() )
        addr_4byte_mode = TRUE;  // 4-byte mode
    else
        addr_4byte_mode = FALSE; // 3-byte mode

    // Chip select go low to start a flash command
    CS_Low();

    // Write READ command and address
    SendByte( FLASH_CMD_READ, SIO );
    SendFlashAddr( flash_address, SIO, addr_4byte_mode );

    // Set a loop to read data into buffer
    for( index=0; index < byte_length; index++ )
    {
        // Read data one byte at a time
        *(target_address + index) = GetByte( SIO );
    }

    // Chip select go high to end a flash command
    CS_High();

    return FlashOperationSuccess;
}

/*
 * Function:       CMD_2READ
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 * Description:    The 2READ instruction enable double throughput of Serial
 *                 Flash in read mode
 * Return Message: FlashAddressInvalid, FlashOperationSuccess
 */
ReturnMsg CMD_2READ( uint32 flash_address, uint8 *target_address, uint32 byte_length )
{
    uint32 index;
    uint8  addr_4byte_mode;

    // Check flash address
    if( flash_address > FlashSize ) return FlashAddressInvalid;

    // Check 3-byte or 4-byte mode
    if( IsFlash4Byte() )
        addr_4byte_mode = TRUE;  // 4-byte mode
    else
        addr_4byte_mode = FALSE; // 3-byte mode

    // Chip select go low to start a flash command
    CS_Low();

    // Write 2-I/O Read command and address
    SendByte( FLASH_CMD_2READ, SIO );
    SendFlashAddr( flash_address, DIO, addr_4byte_mode );
    InsertDummyCycle( 4 );                    // Wait 4 dummy cycle

    // Set a loop to read data into data buffer
    for( index=0; index < byte_length; index++ )
    {
        *(target_address + index) = GetByte( DIO );
    }

    // Chip select go high to end a flash command
    CS_High();

    return FlashOperationSuccess;
}

/*
 * Function:       CMD_4READ
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 * Description:    The 4READ instruction enable quad throughput of
 *                 Serial Flash in read mode.
 * Return Message: FlashAddressInvalid, FlashQuadNotEnable, FlashOperationSuccess
 */
ReturnMsg CMD_4READ( uint32 flash_address, uint8 *target_address, uint32 byte_length )
{
    uint32 index=0;
    uint8  addr_4byte_mode;

    // Check flash address
    if( flash_address > FlashSize ) return FlashAddressInvalid;

    // Check QE bit
    if( IsFlashQIO() != TRUE )  return FlashQuadNotEnable;

    // Check 3-byte or 4-byte mode
    if( IsFlash4Byte() )
        addr_4byte_mode = TRUE;  // 4-byte mode
    else
        addr_4byte_mode = FALSE; // 3-byte mode

    // Chip select go low to start a flash command
    CS_Low();

    // Write 4-I/O Read Array command
    SendByte( FLASH_CMD_4READ, SIO );
    SendFlashAddr( flash_address, QIO, addr_4byte_mode );
    InsertDummyCycle ( 6 );            // Wait 6 dummy cycle

    // Set a loop to read data into flash's buffer
    for( index=0; index < byte_length; index=index + 1 )
    {
        *(target_address + index) = GetByte( QIO );
    }

    // Chip select go high to end a flash command
    CS_High();

    return FlashOperationSuccess;
}

/*
 * Function:       CMD_DREAD
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 * Description:    The DREAD instruction enable double throughput of Serial
 *                 Flash in read mode
 * Return Message: FlashAddressInvalid, FlashOperationSuccess
 */
ReturnMsg CMD_DREAD( uint32 flash_address, uint8 *target_address, uint32 byte_length )
{
    uint32 index;
    uint8  addr_4byte_mode;

    // Check flash address
    if( flash_address > FlashSize ) return FlashAddressInvalid;

    // Check 3-byte or 4-byte mode
    if( IsFlash4Byte() )
        addr_4byte_mode = TRUE;    // 4-byte mode
    else
        addr_4byte_mode = FALSE;   // 3-byte mode

    // Chip select go low to start a flash command
    CS_Low();

    // Write 2-I/O Read command and address
    SendByte( FLASH_CMD_DREAD, SIO );
    SendFlashAddr( flash_address, SIO, addr_4byte_mode );
    InsertDummyCycle( 8 );                    // Wait 8 dummy cycle

    // Set a loop to read data into data buffer
    for( index=0; index < byte_length; index++ )
    {
        *(target_address + index) = GetByte( DIO );
    }

    // Chip select go high to end a flash command
    CS_High();

    return FlashOperationSuccess;
}

/*
 * Function:       CMD_QREAD
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 * Description:    The QREAD instruction enable quad throughput of
 *                 Serial Flash in read mode.
 * Return Message: FlashAddressInvalid, FlashQuadNotEnable, FlashOperationSuccess
 */
ReturnMsg CMD_QREAD( uint32 flash_address, uint8 *target_address, uint32 byte_length )
{
    uint32 index=0;
    uint8  addr_4byte_mode;

    // Check flash address
    if( flash_address > FlashSize ) return FlashAddressInvalid;

    // Check QE bit
    if( IsFlashQIO() != TRUE )  return FlashQuadNotEnable;

    // Check 3-byte or 4-byte mode
    if( IsFlash4Byte() )
        addr_4byte_mode = TRUE;  // 4-byte mode
    else
        addr_4byte_mode = FALSE; // 3-byte mode

    // Chip select go low to start a flash command
    CS_Low();

    // Write 4-I/O Read Array command
    SendByte( FLASH_CMD_QREAD, SIO );
    SendFlashAddr( flash_address, SIO, addr_4byte_mode );
    InsertDummyCycle ( 8 );            // Wait 8 dummy cycle

    // Set a loop to read data into flash's buffer
    for( index=0; index < byte_length; index=index+1)
    {
        *(target_address + index) = GetByte( QIO );
    }

    // Chip select go high to end a flash command
    CS_High();

    return FlashOperationSuccess;
}

/*
 * Function:       CMD_FASTREAD
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 * Description:    The FASTREAD instruction is for quickly reading data out.
 * Return Message: FlashAddressInvalid, FlashOperationSuccess
 */
ReturnMsg CMD_FASTREAD( uint32 flash_address, uint8 *target_address, uint32 byte_length )
{
    uint32 index;
    uint8  addr_4byte_mode;

    // Check flash address
    if( flash_address > FlashSize ) return FlashAddressInvalid;

    // Check 3-byte or 4-byte mode
    if( IsFlash4Byte() )
        addr_4byte_mode = TRUE;  // 4-byte mode
    else
        addr_4byte_mode = FALSE; // 3-byte mode

    // Chip select go low to start a flash command
    CS_Low();

    // Write Fast Read command, address and dummy cycle
    SendByte( FLASH_CMD_FASTREAD, SIO );
    SendFlashAddr( flash_address, SIO, addr_4byte_mode );
    InsertDummyCycle ( 8 );          // Wait dummy cycle

    // Set a loop to read data into data buffer
    for( index=0; index < byte_length; index++ )
    {
        *(target_address + index) = GetByte( SIO );
    }

    // Chip select go high to end a flash command
    CS_High();

    return FlashOperationSuccess;
}


/*
 * Function:       CMD_RDDMC
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 * Description:    RDDMC can retrieve the operating characteristics, structure
 *                 and vendor-specified information such as identifying information,
 *                 memory size, operating voltages and timinginformation of device
 * Return Message: FlashAddressInvalid, FlashOperationSuccess
 */
ReturnMsg CMD_RDDMC( uint32 flash_address, uint8 *target_address, uint32 byte_length )
{
    uint32 index;
    uint8  addr_4byte_mode;

    // Check flash address
    if( flash_address > FlashSize ) return FlashAddressInvalid;

    // Check 3-byte or 4-byte mode
    if( IsFlash4Byte() )
        addr_4byte_mode = TRUE;  // 4-byte mode
    else
        addr_4byte_mode = FALSE; // 3-byte mode

    // Chip select go low to start a flash command
    CS_Low();

    // Write Read CFI command
    SendByte( FLASH_CMD_RDDMC, SIO );
    SendFlashAddr( flash_address, SIO, addr_4byte_mode );
    InsertDummyCycle ( 8 );        // Insert dummy cycle

    // Set a loop to read data into data buffer
    for( index=0; index < byte_length; index++ )
    {
        *(target_address + index) = GetByte( SIO );
    }

    // Chip select go high to end a flash command
    CS_High();

    return FlashOperationSuccess;
}

/*
 * Program Command
 */

/*
 * Function:       CMD_WREN
 * Arguments:      None.
 * Description:    The WREN instruction is for setting
 *                 Write Enable Latch (WEL) bit.
 * Return Message: FlashOperationSuccess
 */
ReturnMsg CMD_WREN( void )
{
    // Chip select go low to start a flash command
    CS_Low();

    // Write Enable command = 0x06, Setting Write Enable Latch Bit
    SendByte( FLASH_CMD_WREN, SIO );

    // Chip select go high to end a flash command
    CS_High();

    return FlashOperationSuccess;
}

/*
 * Function:       CMD_WRDI
 * Arguments:      None.
 * Description:    The WRDI instruction is to reset
 *                 Write Enable Latch (WEL) bit.
 * Return Message: FlashOperationSuccess
 */
ReturnMsg CMD_WRDI( void )
{
    // Chip select go low to start a flash command
    CS_Low();

    // Write Disable command = 0x04, resets Write Enable Latch Bit
    SendByte( FLASH_CMD_WRDI, SIO );

    CS_High();

    return FlashOperationSuccess;
}


/*
 * Function:       CMD_PP
 * Arguments:      flash_address, 32 bit flash memory address
 *                 source_address, buffer address of source data to program
 *                 byte_length, byte length of data to programm
 * Description:    The PP instruction is for programming
 *                 the memory to be "0".
 *                 The device only accept the last 256 byte ( one page ) to program.
 *                 If the page address ( flash_address[7:0] ) reach 0xFF, it will
 *                 program next at 0x00 of the same page.
 * Return Message: FlashAddressInvalid, FlashIsBusy, FlashOperationSuccess,
 *                 FlashTimeOut
 */
ReturnMsg CMD_PP( uint32 flash_address, uint8 *source_address, uint32 byte_length )
{
    uint32 index;
    uint8  addr_4byte_mode;

    // Check flash address
    if( flash_address > FlashSize ) return FlashAddressInvalid;

    // Check flash is busy or not
    if( IsFlashBusy() )    return FlashIsBusy;

    // Check 3-byte or 4-byte mode
    if( IsFlash4Byte() )
        addr_4byte_mode = TRUE;  // 4-byte mode
    else
        addr_4byte_mode = FALSE; // 3-byte mode

    // Setting Write Enable Latch bit
    CMD_WREN();

    // Chip select go low to start a flash command
    CS_Low();

    // Write Page Program command
    SendByte( FLASH_CMD_PP, SIO );
    SendFlashAddr( flash_address, SIO, addr_4byte_mode );

    // Set a loop to down load whole page data into flash's buffer
    // Note: only last 256 byte will be programmed
    for( index=0; index < byte_length; index++ )
    {
        SendByte( *(source_address + index), SIO );
    }

    // Chip select go high to end a flash command
    CS_High();

    if( WaitFlashReady( PageProgramCycleTime ) )
        return FlashOperationSuccess;
    else
        return FlashTimeOut;
}


/*
 * Function:       CMD_4PP
 * Arguments:      flash_address, 32 bit flash memory address
 *                 source_address, buffer address of source data to program
 *                 byte_length, byte length of data to programm
 * Description:    The 4PP instruction is for programming
 *                 the memory to be "0".
 *                 The device only accept the last 256 byte ( one page ) to program.
 *                 If the page address ( flash_address[7:0] ) reach 0xFF, it will
 *                 program next at 0x00 of the same page.
 *                 The different between QPP and 4PP is the IO number during sending address
 * Return Message: FlashQuadNotEnable, FlashAddressInvalid, FlashIsBusy,
 *                 FlashOperationSuccess, FlashTimeOut
 */
ReturnMsg CMD_4PP( uint32 flash_address, uint8 *source_address, uint32 byte_length )
{
    uint32 index;
    uint8  addr_4byte_mode;

    // Check QE bit
    if( !IsFlashQIO() ) return FlashQuadNotEnable;

    // Check flash address
    if( flash_address > FlashSize ) return FlashAddressInvalid;

    // Check flash is busy or not
    if( IsFlashBusy() )    return FlashIsBusy;

    // Check 3-byte or 4-byte mode
    if( IsFlash4Byte() )
        addr_4byte_mode = TRUE;  // 4-byte mode
    else
        addr_4byte_mode = FALSE; // 3-byte mode

    // Setting Write Enable Latch bit
    CMD_WREN();

    // Chip select go low to start a flash command
    CS_Low();

    // Write 4-I/O Page Program command
    SendByte( FLASH_CMD_4PP, SIO );
    SendFlashAddr( flash_address, QIO, addr_4byte_mode );

    // Send source data to flash.
    for( index=0; index < byte_length; index++ )
    {
        SendByte( *(source_address + index), QIO );
    }

    // Chip select go high to end a flash command
    CS_High();

    if( WaitFlashReady( PageProgramCycleTime ) )
        return FlashOperationSuccess;
    else
        return FlashTimeOut;
}

/*
 * Function:       CMD_CP
 * Arguments:      flash_address, 32 bit flash memory address
 *                 source_address, buffer address of source data to programm
 *                 byte_length, byte length of data to program
 *                 fsptr, pointer of flash status structure
 * Description:    The CP instruction is for multiple byte program to Flash.
 *                 The device only accept the last 2 byte to program at a time
 *                 (one CP command). It will not roll over during CP mode.
 * Return Message: FlashAddressInvalid, FlashIsBusy, FlashOperationSuccess,
 *                 FlashTimeOut
 */
ReturnMsg CMD_CP( uint32 flash_address, uint8 *source_address, uint32 byte_length, FlashStatus *fsptr )
{
    uint32 index;
    uint8  addr_4byte_mode;

    // Check flash address
    // Note: CP mode will not roll over during CP mode, so the flash address
    //       add byte offset should not grater than flash size.
    if( (flash_address + byte_length) > FlashSize ) return FlashAddressInvalid;

    // Check flash is busy or not
    if( IsFlashBusy() )    return FlashIsBusy;

    // Check 3-byte or 4-byte mode
    if( IsFlash4Byte() )
        addr_4byte_mode = TRUE;  // 4-byte mode
    else
        addr_4byte_mode = FALSE; // 3-byte mode

    // Setting Write Enable Latch bit
    CMD_WREN();

    // Chip select go low to start a flash command
    CS_Low();

    // Write command
    SendByte( FLASH_CMD_CP, SIO );
    SendFlashAddr( flash_address, SIO, addr_4byte_mode );

    // Send two byte
    SendByte( *(source_address ), SIO );
    SendByte( *(source_address + 1 ), SIO );

    // Set a loop to program data into flash's buffer
    for( index=2; index < byte_length; index = index + 2 )
    {
       CS_High();

            // Wait porgram finish
            if( (fsptr->ModeReg&0x80) == 0x80 ){
                // Wait RYBY (SO) pin until program cycle finished.
                if( !WaitRYBYReady( ByteProgramCycleTime ) )
                    return FlashTimeOut;
            }
            else{
                if( !WaitFlashReady( ByteProgramCycleTime ) )
                    return FlashTimeOut;
            }

        CS_Low();

        // Write Page Program command
        SendByte( FLASH_CMD_CP, SIO );

        // Address incrase one automatically

        // Send two byte
        SendByte( *(source_address + index), SIO );
        SendByte( *(source_address + index + 1), SIO );
    }

    // Chip select go high to end a flash command
    CS_High();

    // End CP mode
    CMD_WRDI();

    if( WaitFlashReady( ByteProgramCycleTime ) )
        return FlashOperationSuccess;
    else
        return FlashTimeOut;
}
/*
 * Erase Command
 */

/*
 * Function:       CMD_SE
 * Arguments:      flash_address, 32 bit flash memory address
 * Description:    The SE instruction is for erasing the data
 *                 of the chosen sector (4KB) to be "1".
 * Return Message: FlashAddressInvalid, FlashIsBusy, FlashOperationSuccess,
 *                 FlashTimeOut
 */
ReturnMsg CMD_SE( uint32 flash_address )
{
    uint8  addr_4byte_mode;

    // Check flash address
    if( flash_address > FlashSize ) return FlashAddressInvalid;

    // Check flash is busy or not
    if( IsFlashBusy() )    return FlashIsBusy;

    // Check 3-byte or 4-byte mode
    if( IsFlash4Byte() )
        addr_4byte_mode = TRUE;  // 4-byte mode
    else
        addr_4byte_mode = FALSE; // 3-byte mode

    // Setting Write Enable Latch bit
    CMD_WREN();

    // Chip select go low to start a flash command
    CS_Low();

    //Write Sector Erase command = 0x20;
    SendByte( FLASH_CMD_SE, SIO );
    SendFlashAddr( flash_address, SIO, addr_4byte_mode );

    // Chip select go high to end a flash command
    CS_High();

    if( WaitFlashReady( SectorEraseCycleTime ) )
        return FlashOperationSuccess;
    else
        return FlashTimeOut;
}

/*
 * Function:       CMD_BE32K
 * Arguments:      flash_address, 32 bit flash memory address
 * Description:    The BE32K instruction is for erasing the data
 *                 of the chosen sector (32KB) to be "1".
 * Return Message: FlashAddressInvalid, FlashIsBusy, FlashOperationSuccess,
 *                 FlashTimeOut
 */
ReturnMsg CMD_BE32K( uint32 flash_address )
{
    uint8  addr_4byte_mode;

    // Check flash address
    if( flash_address > FlashSize ) return FlashAddressInvalid;

    // Check flash is busy or not
    if( IsFlashBusy() )    return FlashIsBusy;

    // Check 3-byte or 4-byte mode
    if( IsFlash4Byte() )
        addr_4byte_mode = TRUE;  // 4-byte mode
    else
        addr_4byte_mode = FALSE; // 3-byte mode

    // Setting Write Enable Latch bit
    CMD_WREN();

    // Chip select go low to start a flash command
    CS_Low();

    //Write Block Erase32KB command;
    SendByte( FLASH_CMD_BE32K, SIO );
    SendFlashAddr( flash_address, SIO, addr_4byte_mode );

    // Chip select go high to end a flash command
    CS_High();

    if( WaitFlashReady( BlockErase32KCycleTime ) )
        return FlashOperationSuccess;
    else
        return FlashTimeOut;
}

/*
 * Function:       CMD_BE
 * Arguments:      flash_address, 32 bit flash memory address
 * Description:    The BE instruction is for erasing the data
 *                 of the chosen sector (64KB) to be "1".
 * Return Message: FlashAddressInvalid, FlashIsBusy, FlashOperationSuccess,
 *                 FlashTimeOut
 */
ReturnMsg CMD_BE( uint32 flash_address )
{
    uint8  addr_4byte_mode;

    // Check flash address
    if( flash_address > FlashSize ) return FlashAddressInvalid;

    // Check flash is busy or not
    if( IsFlashBusy() )    return FlashIsBusy;

    // Check 3-byte or 4-byte mode
    if( IsFlash4Byte() )
        addr_4byte_mode = TRUE;  // 4-byte mode
    else
        addr_4byte_mode = FALSE; // 3-byte mode

    // Setting Write Enable Latch bit
    CMD_WREN();

    // Chip select go low to start a flash command
    CS_Low();

    //Write Block Erase command = 0xD8;
    SendByte( FLASH_CMD_BE, SIO );
    SendFlashAddr( flash_address, SIO, addr_4byte_mode );

    // Chip select go high to end a flash command
    CS_High();

    if( WaitFlashReady( BlockEraseCycleTime ) )
        return FlashOperationSuccess;
    else
        return FlashTimeOut;
}

/*
 * Function:       CMD_CE
 * Arguments:      None.
 * Description:    The CE instruction is for erasing the data
 *                 of the whole chip to be "1".
 * Return Message: FlashIsBusy, FlashOperationSuccess, FlashTimeOut
 */
ReturnMsg CMD_CE( void )
{
    // Check flash is busy or not
    if( IsFlashBusy() )    return FlashIsBusy;

    // Setting Write Enable Latch bit
    CMD_WREN();

    // Chip select go low to start a flash command
    CS_Low();

    //Write Chip Erase command = 0x60;
    SendByte( FLASH_CMD_CE, SIO );

    // Chip select go high to end a flash command
    CS_High();

    if( WaitFlashReady( ChipEraseCycleTime ) )
        return FlashOperationSuccess;
    else
        return FlashTimeOut;
}


/*
 * Mode setting Command
 */

/*
 * Function:       CMD_DP
 * Arguments:      None.
 * Description:    The DP instruction is for setting the
 *                 device on the minimizing the power consumption.
 * Return Message: FlashOperationSuccess
 */
ReturnMsg CMD_DP( void )
{
    // Chip select go low to start a flash command
    CS_Low();

    // Deep Power Down Mode command
    SendByte( FLASH_CMD_DP, SIO );

    // Chip select go high to end a flash command
    CS_High();

    return FlashOperationSuccess;
}

/*
 * Function:       CMD_RDP
 * Arguments:      None.
 * Description:    The Release from RDP instruction is
 *                 putting the device in the Stand-by Power mode.
 * Return Message: FlashOperationSuccess
 */
ReturnMsg CMD_RDP( void )
{
    // Chip select go low to start a flash command
    CS_Low();

    // Deep Power Down Mode command
    SendByte( FLASH_CMD_RDP, SIO );

    // Chip select go high to end a flash command
    CS_High();

    return FlashOperationSuccess;
}

/*
 * Function:       CMD_ENSO
 * Arguments:      None.
 * Description:    The ENSO instruction is for entering the secured OTP mode.
 * Return Message: FlashOperationSuccess
 */
ReturnMsg CMD_ENSO( void )
{
    // Chip select go low to start a flash command
    CS_Low();

    // Write ENSO command
    SendByte( FLASH_CMD_ENSO, SIO );

    // Chip select go high to end a flash command
    CS_High();

    return FlashOperationSuccess;
}

/*
 * Function:       CMD_EXSO
 * Arguments:      None.
 * Description:    The EXSO instruction is for exiting the secured OTP mode.
 * Return Message: FlashOperationSuccess
 */
ReturnMsg CMD_EXSO( void )
{
    // Chip select go low to start a flash command
    CS_Low();

    // Write EXSO command = 0xC1
    SendByte( FLASH_CMD_EXSO, SIO );

    // Chip select go high to end a flash command
    CS_High();

    return FlashOperationSuccess;
}

/*
 * Function:       CMD_ESRY
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    The ESRY instruction is for outputting the ready/busy
 *                 status to SO during CP mode.
 * Return Message: FlashOperationSuccess
 */
ReturnMsg CMD_ESRY( FlashStatus *fsptr )
{
    // Chip select go low to start a flash command
    CS_Low();

    // Write command
    SendByte( FLASH_CMD_ESRY, SIO );

    // Chip select go high to end a flash command
    CS_High();

    fsptr->ModeReg = (fsptr->ModeReg|0x80);

    return FlashOperationSuccess;
}

/*
 * Function:       CMD_DSRY
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    The DSRY instruction is for resetting ESRY during CP mode.
 * Return Message: FlashOperationSuccess
 */
ReturnMsg CMD_DSRY( FlashStatus *fsptr )
{
    // Chip select go low to start a flash command
    CS_Low();

    // Write command
    SendByte( FLASH_CMD_DSRY, SIO );

    // Chip select go high to end a flash command
    CS_High();

    fsptr->ModeReg = (fsptr->ModeReg&0x7f);

    return FlashOperationSuccess;
}

/*
 * Function:       CMD_WPSEL
 * Arguments:      None.
 * Description:    When the system accepts and executes WPSEL instruction,
 *                 the bit 7 in security register (WPSEL) will be set.
 * Return Message: FlashOperationSuccess, FlashWriteRegFailed, FlashTimeOut
 */
ReturnMsg CMD_WPSEL( void )
{
    uint8  gDataBuffer;

    // Setting Write Enable Latch bit
    CMD_WREN();

    // Chip select go low to start a flash command
    CS_Low();

    // Send WPSEL command
    SendByte( FLASH_CMD_WPSEL, SIO );

    // Chip select go high to end a flash command
    CS_High();

    if( WaitFlashReady( WriteStatusRegCycleTime ) ){

        CMD_RDSCUR( &gDataBuffer );

        // Check WPSEL bit
        if( (gDataBuffer & FLASH_WPSEL_MASK) == FLASH_WPSEL_MASK )
            return FlashOperationSuccess;
        else
            return FlashWriteRegFailed;
    }
    else
        return FlashTimeOut;

}

/*
 * Function:       CMD_EN4B
 * Arguments:      None
 * Description:    The EN4B instruction is for entering 4-byte mode.
 * Return Message: FlashOperationSuccess, FlashWriteRegFailed
 */
ReturnMsg CMD_EN4B( void )
{
    // Chip select go low to start a flash command
    CS_Low();

    // Write command
    SendByte( FLASH_CMD_EN4B, SIO );

    // Chip select go high to end a flash command
    CS_High();

    if( IsFlash4Byte() )
        return FlashOperationSuccess;
    else
        return FlashWriteRegFailed;
}

/*
 * Function:       CMD_EX4B
 * Arguments:      None
 * Description:    The EX4B instruction is for exiting 4-byte mode.
 * Return Message: FlashOperationSuccess, FlashWriteRegFailed
 */
ReturnMsg CMD_EX4B( void )
{
    // Chip select go low to start a flash command
    CS_Low();

    // Write command
    SendByte( FLASH_CMD_EX4B, SIO );

    // Chip select go high to end a flash command
    CS_High();

    if( IsFlash4Byte() )
        return FlashWriteRegFailed;
    else
        return FlashOperationSuccess;

}


/*
 * Security Command
 */

/*
 * Function:       CMD_SBLK
 * Arguments:      flash_address, 32 bit flash memory address
 * Description:    The SBLK instruction is for write protection a specified
 *                 block (or sector) of flash memory.
 *                 This instructions are only effective after WPSEL was executed.
 * Return Message: FlashAddressInvalid, FlashIsBusy, FlashOperationSuccess,
 *                 FlashTimeOut
 */
ReturnMsg CMD_SBLK( uint32 flash_address )
{
    uint8  addr_4byte_mode;

    // Check flash address
    if( flash_address > FlashSize ) return FlashAddressInvalid;

    // Check flash is busy or not
    if( IsFlashBusy() )    return FlashIsBusy;

    // Check 3-byte or 4-byte mode
    if( IsFlash4Byte() )
        addr_4byte_mode = TRUE;  // 4-byte mode
    else
        addr_4byte_mode = FALSE; // 3-byte mode

    // Setting Write Enable Latch bit
    CMD_WREN();

    // Chip select go low to start a flash command
    CS_Low();

    // Write SBLK command = 0x36;
    SendByte( FLASH_CMD_SBLK, SIO );
    SendFlashAddr( flash_address, SIO, addr_4byte_mode );

    // Chip select go high to end a flash command
    CS_High();

    if( WaitFlashReady( WriteStatusRegCycleTime ) )
        return FlashOperationSuccess;
    else
        return FlashTimeOut;
}

/*
 * Function:       CMD_SBULK
 * Arguments:      flash_address, 32 bit flash memory address
 * Description:    The SBULK instruction will cancel the block (or sector)
 *                 write protection state.
 *                 This instructions are only effective after WPSEL was executed.
 * Return Message: FlashAddressInvalid, FlashIsBusy, FlashOperationSuccess,
 *                 FlashTimeOut
 */
ReturnMsg CMD_SBULK( uint32 flash_address )
{
    uint8  addr_4byte_mode;

    // Check flash address
    if( flash_address > FlashSize ) return FlashAddressInvalid;

    // Check flash is busy or not
    if( IsFlashBusy() )    return FlashIsBusy;

    // Check 3-byte or 4-byte mode
    if( IsFlash4Byte() )
        addr_4byte_mode = TRUE;  // 4-byte mode
    else
        addr_4byte_mode = FALSE; // 3-byte mode

    // Setting Write Enable Latch bit
    CMD_WREN();

    // Chip select go low to start a flash command
    CS_Low();

    // Write SBLUK command = 0x39;
    SendByte( FLASH_CMD_SBULK, SIO );
    SendFlashAddr( flash_address, SIO, addr_4byte_mode );

    // Chip select go high to end a flash command
    CS_High();

    if( WaitFlashReady( WriteStatusRegCycleTime ) )
        return FlashOperationSuccess;
    else
        return FlashTimeOut;
}

/*
 * Function:       CMD_RDBLOCK
 * Arguments:      flash_address, 32 bit flash memory address
 *                 protect_flag, TRUE:  block (or sector) is protected
 *                               FALSE: block (or sector) is unprotected
 * Description:    The RDBLOCK instruction is for reading the status of protection
 *                 lock of a specified block (or sector).
 *                 This instruction is only effective after WPSEL was executed.
 * Return Message: FlashAddressInvalid, FlashOperationSuccess, FlashTimeOut
 */
ReturnMsg CMD_RDBLOCK( uint32 flash_address, BOOL *protect_flag )
{
    uint8  lock_flag;
    uint8  addr_4byte_mode;

    // Check flash address
    if( flash_address > FlashSize ) return FlashAddressInvalid;

    // Check 3-byte or 4-byte mode
    if( IsFlash4Byte() )
        addr_4byte_mode = TRUE;  // 4-byte mode
    else
        addr_4byte_mode = FALSE; // 3-byte mode

    // Chip select go low to start a flash command
    CS_Low();

    SendByte( FLASH_CMD_RDBLOCK, SIO );       // Write RDBLOCK command
    SendFlashAddr( flash_address, SIO, addr_4byte_mode );
    lock_flag = GetByte( SIO );               // get 8 bits data ouput from Flash

    // Chip select go high to end a flash command
    CS_High();


    // Check protect status
    if( lock_flag & BLOCK_PROTECT_MASK == BLOCK_PROTECT_MASK )
        *protect_flag = TRUE;
    else
        *protect_flag = FALSE;

    return FlashOperationSuccess;
}

/*
 * Function:       CMD_GBLK
 * Arguments:      None.
 * Description:    The GBLK instruction is for enablethe lock protection
 *                 block of the whole chip.
 *                 This instruction is only effective after WPSEL was executed.
 * Return Message: FlashIsBusy, FlashOperationSuccess, FlashTimeOut
 */
ReturnMsg CMD_GBLK( void )
{
    // Check flash is busy or not
    if( IsFlashBusy() )    return FlashIsBusy;

    // Setting Write Enable Latch bit
    CMD_WREN();

    // Chip select go low to start a flash command
    CS_Low();

    // Write GBLK command
    SendByte( FLASH_CMD_GBLK, SIO );

    // Chip select go high to end a flash command
    CS_High();

    if( WaitFlashReady( WriteStatusRegCycleTime ) )
        return FlashOperationSuccess;
    else
        return FlashTimeOut;
}

/*
 * Function:       CMD_GBULK
 * Arguments:      None.
 * Description:    The GBULK instruction is for disable the lock protection
 *                 block of the whole chip.
 *                 This instruction is only effective after WPSEL was executed.
 * Return Message: FlashIsBusy, FlashOperationSuccess, FlashTimeOut
 */
ReturnMsg CMD_GBULK( void )
{
    // Check flash is busy or not
    if( IsFlashBusy() )    return FlashIsBusy;

    // Setting Write Enable Latch bit
    CMD_WREN();

    // Chip select go low to start a flash command
    CS_Low();

    // Write GBULK command = 0x98;
    SendByte( FLASH_CMD_GBULK, SIO );

    // Chip select go high to end a flash command
    CS_High();

    if( WaitFlashReady( WriteStatusRegCycleTime ) )
        return FlashOperationSuccess;
    else
        return FlashTimeOut;
}


