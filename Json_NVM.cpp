#include "Json_NVM.h"

Json_NVM::Json_NVM(unsigned int _NVM_offset, unsigned int _NVM_size)
{
  NVM_offset = _NVM_offset;
  NVM_size = _NVM_size;
}

unsigned int Json_NVM::size()
{
  return NVM_size;
}

bool Json_NVM::readJSON(DynamicJsonDocument& doc)
{
    return read_NVM(doc);
}

// Low level EEPROM storage functions:
bool Json_NVM::read_NVM(DynamicJsonDocument& doc)
{
  EEPROM.begin(NVM_offset + NVM_size);
  EepromStream eepromStream(NVM_offset, NVM_size);
  DeserializationError error = deserializeJson(doc, eepromStream);
  if (error)
  {
    printf("ERROR during deserialization!!!\n");
    EEPROM.end();
    return false;
  }
  Serial.printf("Read back from EEPROM:\n");
  serializeJson(doc, Serial);
  Serial.printf("\n");
  EEPROM.end();
  delay(100);
  return true;
}

void Json_NVM::write_NVM(DynamicJsonDocument& doc)
{
  EEPROM.begin(NVM_offset + NVM_size);
  EepromStream eepromStream(NVM_offset, NVM_size);
  Serial.printf("Serialized JSON:\n");
  serializeJson(doc, Serial);
  Serial.printf("\n");
  serializeJson(doc, eepromStream);
  EEPROM.end();
  delay(100);
}

void Json_NVM::dump_NVM()
{
  dump_NVM(NVM_offset, NVM_size);
}

void Json_NVM::dump_NVM(unsigned int _NVM_offset, unsigned int _NVM_size)
{
  unsigned int address, col=0;
  byte value;
  EEPROM.begin(_NVM_offset + _NVM_size);
  for (int n=0; n<NVM_size; n++)
  {
    address = NVM_offset + n;
    value = EEPROM.read(_NVM_offset + n);
    if (col == 0)
      Serial.printf("%04X  %02X ", address, value);
    else if (col<7 || (col>7 && col<15))
      Serial.printf("%02X ", value);
    else if (col==7)
      Serial.printf("%02X  ", value);
    else if (col == 15)
      Serial.printf("%02X\n", value);
    col++;
    if (col>15)
      col = 0;

  }
  EEPROM.end();
  delay(100);
}
