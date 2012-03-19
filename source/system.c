#include "system.h"

static uint8_t uiActiveSector;
static uint16_t uiDataPtr;

/**
 * Inits the Latches to all-Zero values.
 * This should be done once.
 */
void adr_init(void)
{
	//Configure output
	sbi(DDRD,PD4);
	sbi(DDRD,PD5);
	sbi(DDRD,PD7);

	sbi(DDRB,PB4);
	sbi(PORTB,PB4);

	adr_set(0x00, 1);
	adr_set(0x00, 2);
	adr_set(0x00, 3);
	adr_set(0x00, 4);
}
/**
 * Reset all latches to zero (low).
 */
void adr_reset()
{
	adr_set(0x00, 1);
	adr_set(0x00, 2);
	adr_set(0x00, 3);
	adr_set(0x00, 4);
}

/**
 * sets address to a specific latch
 * @param cAddress the address to set
 * @param uiLatchNr	the latch number
 */
void adr_set(char cAddress, uint8_t uiLatchNr)
{
	bus_output();

	//select wanted latch - 3 bits input
	if (uiLatchNr & 0x01)
	{
		sbi(PORTD,PD4);
	}
	else
	{
		cbi(PORTD,PD4);
	}

	uiLatchNr = (uiLatchNr >> 1);
	if (uiLatchNr & 0x01)
	{
		sbi(PORTD,PD5);
	}
	else
	{
		cbi(PORTD,PD5);
	}

	uiLatchNr = (uiLatchNr >> 1);
	if (uiLatchNr & 0x01)
	{
		sbi(PORTD,PD7);
	}
	else
	{
		cbi(PORTD,PD7);
	}

	//set address to databus
	PORTC = cAddress;

	//select Latch
	//enable 3-8 decoder
	cbi(PORTB,PB4);
	_delay_us(10);
	//to clean up, disable 3-8 decoder
	sbi(PORTB,PB4);
}

static void mem_command(uint8_t uiAddressL0,uint8_t uiAddressL1,uint8_t uiAddressL2, uint8_t uiData)
{
	bus_output();
	adr_set(uiAddressL0,0);
	adr_set(uiAddressL1,1);
	adr_set(uiAddressL2,2);

	cbi(PORTB,WR);

	PORTC = uiData;

	sbi(PORTB,WR);

	adr_set(0x00,0);
	adr_set(0x00,1);
	adr_set(0x00,2);
}

static void mem_write_byte(uint8_t uiSector,uint16_t uiAddress, uint8_t uiData)
{
	//unlock command
	mem_command(0x55, 0x05, 0x00, 0xAA);
	mem_command(0xAA, 0x02, 0x00, 0x55);

	//write command
	mem_command(0x55, 0x05, 0x00, 0xA0);

	//write data - this is like writing a command
	mem_command(uiAddress, uiAddress>>8, uiSector, uiData);
}

void mem_chip_erase(void)
{
	mem_command(0x55, 0x05, 0x00, 0xAA);
	mem_command(0xAA, 0x02, 0x00, 0x55);
	mem_command(0x55, 0x05, 0x00, 0x80);
	mem_command(0x55, 0x05, 0x00, 0xAA);
	mem_command(0xAA, 0x02, 0x00, 0x55);
	mem_command(0x55, 0x05, 0x00, 0x10);

	bus_input();
	while(PINC != 0xFF){};

	//tag sector 0 as currently active in state field
	mem_write_byte(0x00,0x0000,0x0F);
}

void mem_sector_erase(uint8_t uiSector)
{
	mem_command(0x55, 0x05, 0x00, 0xAA);
	mem_command(0xAA, 0x02, 0x00, 0x55);
	mem_command(0x55, 0x05, 0x00, 0x80);
	mem_command(0x55, 0x05, 0x00, 0xAA);
	mem_command(0xAA, 0x02, 0x00, 0x55);
	mem_command(0x00, 0x00, uiSector, 0x30);

	bus_input();
	while(PINC != 0xFF){};
}
static uint8_t mem_read_byte(uint8_t uiSector,uint16_t uiAddress)
{
	adr_set(uiAddress,0);
	adr_set(uiAddress>>8,1);
	adr_set(uiSector,2);

	PORTC = 0x00;
	bus_input();

	cbi(PORTB,RD);
	_delay_us(1);
	uint8_t uiResult = PINC;
	sbi(PORTB,RD);

	adr_set(0x00,0);
	adr_set(0x00,1);
	adr_set(0x00,2);

	return uiResult;
}

static uint8_t mem_find_active(void)
{
	uint8_t uiResult = 0xFF;
	//search for currently active sector
	for (uint8_t i=0;i<8;i++)
	{
		//check status field
		if (mem_read_byte(i,0x0000) == 0x0F)
		{
			//we have found active sector
			uiResult = i;
			break;
		}
	}

	return uiResult;
}
static void mem_alloc(uint8_t uiSize)
{
	//is enough free space available? - we need data size + 1 byte for size + 1 byte free after writing
	if((0xFFFF - uiDataPtr) >= (uiSize+2)) return;

	//if we get here, we need to change the sector!
	//first disable the old one
	mem_write_byte(uiActiveSector,0x00,0x00);

	uiActiveSector = (uiActiveSector+1) % 8;

	if (mem_read_byte(uiActiveSector,0x0000) != 0xFF)
	{
		//we need to erase the sector
		mem_sector_erase(uiActiveSector);
	}

	//mark this sector as active
	mem_write_byte(uiActiveSector,0x0000,0x0F);

	uiDataPtr = 0x0001;
}
void mem_write_data(char* pData, uint8_t size)
{
	if(size == 0) return;
	mem_alloc(size);

	//write data to data location
	mem_write_byte(uiActiveSector,uiDataPtr,size);
	uiDataPtr++;
	for (uint8_t i =0; i<size;i++)
	{
		mem_write_byte(uiActiveSector,uiDataPtr,*pData);
		uiDataPtr++;
		pData++;
	}
}
static uint8_t mem_find_empty(void)
{
	uint8_t uiResult = 0xFF;
	for (uint8_t i=0;i<8;i++)
	{
		//check status field
		if (mem_read_byte(i,0x0000) == 0xFF)
		{
			//we have found empty sector
			uiResult = i;
			break;
		}
	}
	return uiResult;
}

void mem_print_sector(uint8_t uiSector)
{
	uint16_t uiPtr = 0x0001;
	while(mem_read_byte(uiSector,uiPtr) != 0xFF)
	{
		uint8_t uiSize = mem_read_byte(uiSector,uiPtr);
		for (uint8_t i=0;i<=uiSize;i++)
		{
			uiPtr++;
			uart_send_char(mem_read_byte(uiSector,uiPtr));
		}
	}
}

static uint16_t mem_find_first_free(uint8_t uiSector)
{
	uint16_t uiPtr = 0x0001;
	while(mem_read_byte(uiSector,uiPtr) != 0xFF)
	{
		uint8_t uiSize = mem_read_byte(uiSector,uiPtr);
		uiPtr += uiSize + 1;
	}
	return uiPtr;
}
/**
 * Inits the system for memory access.
 */
void mem_init(void)
{
	sbi(DDRB,WR);
	sbi(DDRB,RD);
	sbi(PORTB,RD);
	sbi(PORTB,WR);

	uint8_t uiActive = mem_find_active();
	if (uiActive != 0xFF)
	{
		//we have found an active sector
		uart_send("found active");
		uiActiveSector = uiActive;
		uiDataPtr = mem_find_first_free(uiActiveSector);
	} else {
		uiActive = mem_find_empty();
		if (uiActive != 0xFF)
		{
			uiActiveSector = uiActive;
			uart_send("found empty");
			//tag sector as "in use"
			mem_write_byte(uiActiveSector,0x0000,0x0F);
			uiDataPtr =0x0001;
		} else {
			uart_send("Performing chip erase");
			//we did not found anything
			mem_chip_erase();
			uiActiveSector = 0x00;
			mem_write_byte(uiActiveSector,0x0000,0x0F);
			uiDataPtr = 0x0001;
		}
	}
	uart_send("Current Ptrs:");
	uart_send_char(uiActiveSector);
	uart_send_char(uiDataPtr >> 8);
	uart_send_char(uiDataPtr);
}
