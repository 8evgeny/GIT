#include <OneWire.h>


OneWire::OneWire()
{
//  _GPIO_Port = GPIO_Port;
//  _GPIO_Pin  = GPIO_Pin;

//  reset_search();
}

void OneWire::Init(GPIO_TypeDef *GPIO_Port, uint16_t GPIO_Pin)
{
  _GPIO_Port = GPIO_Port;
  _GPIO_Pin  = GPIO_Pin;
  _SetSpeed(Speed_High);
  _SetPin(_GPIO_Port, _GPIO_Pin, Output, Pull_Up);
  _SetSpeed(Speed_Low);
  uint8_t I = 0;
  uint16_t D = _GPIO_Pin;
  while(I < 16) { D = D >> 1; if(D == 0) break; I++; }                          // Подсчитываем номер бита который соответствует настраиваемому выводу

  _GPIO_Pin_Pos = I;
}

// Perform the onewire reset function.  We will wait up to 250uS for
// the bus to come high, if it doesn't then it is broken or shorted
// and we return a 0;
//
// Returns 1 if a device asserted a presence pulse, 0 otherwise.
//

uint8_t OneWire::reset(void)
{
  uint16_t r;
  uint8_t retries = 125;

  __disable_irq();                                          // noInterrupts();
  DIRECT_MODE_INPUT;
  __enable_irq();                                           // interrupts();
  // wait until the wire is high... just in case
  do
  {
    if(--retries == 0) return 0;
    delayMicroseconds(2);
  } while (!DIRECT_READ);

  __disable_irq();                                          // noInterrupts();
  DIRECT_WRITE_LOW;
  DIRECT_MODE_OUTPUT;	                                      // drive output low
  __enable_irq();                                           // interrupts();
  delayMicroseconds(480);
  __disable_irq();                                          // noInterrupts();
  DIRECT_MODE_INPUT;	                                      // allow it to float
  delayMicroseconds(70);
  r = !DIRECT_READ;
  __enable_irq();                                           // interrupts();
  delayMicroseconds(410);
  return r;
}

//
// Write a bit. Port and bit is used to cut lookup time and provide
// more certain timing.
//
void OneWire::write_bit(uint8_t v)
{
  if(v & 1)
  {
    __disable_irq();
    DIRECT_WRITE_LOW;
    DIRECT_MODE_OUTPUT;
    delayMicroseconds(10);
    DIRECT_WRITE_HIGH;
    __enable_irq();
    delayMicroseconds(55);
  }
  else
  {
    __disable_irq();
    DIRECT_WRITE_LOW;
    DIRECT_MODE_OUTPUT;
    delayMicroseconds(65);
    DIRECT_WRITE_HIGH;
    __enable_irq();
    delayMicroseconds(5);
  }
}


//
// Read a bit. Port and bit is used to cut lookup time and provide
// more certain timing.
//
uint8_t OneWire::read_bit(void)
{
  uint8_t r;

  __disable_irq();
  DIRECT_MODE_OUTPUT;
  DIRECT_WRITE_LOW;
  delayMicroseconds(3);
  DIRECT_MODE_INPUT;
  delayMicroseconds(10);
  r = DIRECT_READ;
  __enable_irq();
  delayMicroseconds(53);
  return r;
}

//
// Write a byte. The writing code uses the active drivers to raise the
// pin high, if you need power after the write (e.g. DS18S20 in
// parasite power mode) then set 'power' to 1, otherwise the pin will
// go tri-state at the end of the write to avoid heating in a short or
// other mishap.
//

void OneWire::write(uint8_t v, uint8_t power )
{
  for(uint8_t I=0; I<8; I++)
  {
    write_bit(v & 0x01);
    v = v >> 1;
  }
  if(!power)
  {
    __disable_irq();
    DIRECT_MODE_INPUT;
    DIRECT_WRITE_LOW;
    __enable_irq();
  }
}


void OneWire::write_bytes(const uint8_t *buf, uint16_t count, bool power)
{
  for (uint16_t i = 0; i < count; i++)
    write(buf[i]);
  if(!power)
  {
    __disable_irq();
    DIRECT_MODE_INPUT;
    DIRECT_WRITE_LOW;
    __disable_irq();
  }
}

//
// Read a byte
//

uint8_t OneWire::read()
{
  uint8_t bitMask;
  uint8_t r = 0;

  for (bitMask = 0x01; bitMask; bitMask <<= 1)
  {
    if(read_bit()) r |= bitMask;
  }
  return r;
}

void OneWire::read_bytes(uint8_t *buf, uint16_t count)
{
  for (uint16_t i = 0; i < count; i++)
    buf[i] = read();
}

//
// Do a ROM select
//
void OneWire::select(const uint8_t rom[8])
{
  uint8_t i;

  write(0x55);           // Choose ROM

  for (i = 0; i < 8; i++)
    write(rom[i]);
}

//
// Do a ROM skip
//
void OneWire::skip()
{
  write(0xCC);           // Skip ROM
}

void OneWire::depower()
{
  __disable_irq();
  DIRECT_MODE_INPUT;
  __enable_irq();
}


void OneWire::reset_search(void)
{
  // reset the search state
  _LastDiscrepancy = 0;
  _LastDeviceFlag = false;
  _LastFamilyDiscrepancy = 0;
  for (int i = 7;; i--)
  {
    _ROM_NO[i] = 0;
    if(i == 0) break;
  }
}

// Setup the search to find the device type 'family_code' on the next call
// to search(*newAddr) if it is present.
//
void OneWire::target_search(uint8_t family_code)
{
  // set the search state to find SearchFamily type devices
  _ROM_NO[0] = family_code;
  for (uint8_t i = 1; i < 8; i++)
    _ROM_NO[i] = 0;
  _LastDiscrepancy = 64;
  _LastFamilyDiscrepancy = 0;
  _LastDeviceFlag = false;
}

//
// Perform a search. If this function returns a '1' then it has
// enumerated the next device and you may retrieve the ROM from the
// OneWire::address variable. If there are no devices, no further
// devices, or something horrible happens in the middle of the
// enumeration then a 0 is returned.  If a new device is found then
// its address is copied to newAddr.  Use OneWire::reset_search() to
// start over.
//
// --- Replaced by the one from the Dallas Semiconductor web site ---
//--------------------------------------------------------------------------
// Perform the 1-Wire Search Algorithm on the 1-Wire bus using the existing
// search state.
// Return TRUE  : device found, ROM number in ROM_NO buffer
//        FALSE : device not found, end of search
//

uint8_t OneWire::search(uint8_t *newAddr, bool search_mode)
{
  uint16_t id_bit_number;
  uint8_t last_zero, rom_byte_number, search_result;
  uint8_t id_bit, cmp_id_bit;

  unsigned char rom_byte_mask, search_direction;

  // initialize for search
  id_bit_number = 1;
  last_zero = 0;
  rom_byte_number = 0;
  rom_byte_mask = 1;
  search_result = 0;

  // if the last call was not the last one
  if(!_LastDeviceFlag)
  {
    // 1-Wire reset
    if(!reset())
    {
      // reset the search
      _LastDiscrepancy = 0;
      _LastDeviceFlag = false;
      _LastFamilyDiscrepancy = 0;
      return false;
    }

    // issue the search command
    if(search_mode == true)
    {
      write(0xF0);                                          // NORMAL SEARCH
    }
    else
    {
      write(0xEC);                                          // CONDITIONAL SEARCH
    }

    // loop to do the search
    do
    {
      // read a bit and its complement
      id_bit = read_bit();
      cmp_id_bit = read_bit();

      // check for no devices on 1-wire
      if((id_bit == 1) && (cmp_id_bit == 1)) break;
      else
      {
        // all devices coupled have 0 or 1
        if(id_bit != cmp_id_bit) search_direction = id_bit;  // bit write value for search
        else
        {
          // if this discrepancy if before the Last Discrepancy
          // on a previous next then pick the same as last time
          if(id_bit_number < _LastDiscrepancy) search_direction = ((_ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
          else
          // if equal to last pick 1, if not then pick 0
          search_direction = (id_bit_number == _LastDiscrepancy);

          // if 0 was picked then record its position in LastZero
          if(search_direction == 0)
          {
            last_zero = id_bit_number;

            // check for Last discrepancy in family
            if(last_zero < 9) _LastFamilyDiscrepancy = last_zero;
          }
        }

        // set or clear the bit in the ROM byte rom_byte_number
        // with mask rom_byte_mask
        if(search_direction == 1) _ROM_NO[rom_byte_number] |= rom_byte_mask;
        else _ROM_NO[rom_byte_number] &= ~rom_byte_mask;

        // serial number search direction write bit
        write_bit(search_direction);

        // increment the byte counter id_bit_number
        // and shift the mask rom_byte_mask
        id_bit_number++;
        rom_byte_mask <<= 1;

        // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
        if(rom_byte_mask == 0)
        {
          rom_byte_number++;
          rom_byte_mask = 1;
        }
      }
    } while (rom_byte_number < 8);  // loop until through all ROM bytes 0-7

    // if the search was successful then
    if(!(id_bit_number < 65))
    {
      // search successful so set LastDiscrepancy,LastDeviceFlag,search_result
      _LastDiscrepancy = last_zero;

      // check for last device
      if(_LastDiscrepancy == 0) _LastDeviceFlag = true;

      search_result = true;
    }
  }

  // if no device found then reset counters so next 'search' will be like a first
  if(!search_result || !_ROM_NO[0])
  {
    _LastDiscrepancy = 0;
    _LastDeviceFlag = false;
    _LastFamilyDiscrepancy = 0;
    search_result = true;
  }
  else
  {
    for (int i = 0; i < 8; i++)
      newAddr[i] = _ROM_NO[i];
  }
  return search_result;
}




//
// Compute a Dallas Semiconductor 8 bit CRC directly.
// this is much slower, but much smaller, than the lookup table.
//

uint8_t OneWire::crc8(const uint8_t *addr, uint8_t len)
{
  uint8_t crc = 0;

  while (len--)
  {
    uint8_t inbyte = *addr++;
    for (uint8_t i = 8; i; i--)
    {
      uint8_t mix = (crc ^ inbyte) & 0x01;
      crc >>= 1;
      if(mix) crc ^= 0x8C;
      inbyte >>= 1;
    }

  }
  return crc;
}

