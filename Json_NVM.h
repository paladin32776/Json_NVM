#include <EEPROM.h>
#include <ArduinoJson.h>
#include "StreamUtils.h"


class Json_NVM
{
  private:
    unsigned int NVM_offset, NVM_size;
    // Template functions to assemble JSON object from variable number of input parameters:
    template<typename Tparameter, typename Tvalue>
    void assembleJSON(DynamicJsonDocument& doc, Tparameter parameter, Tvalue value)
    {
      doc[parameter]=value;
    }
    template<typename Tparameter, typename Tvalue, typename... Args>
    void assembleJSON(DynamicJsonDocument& doc, Tparameter parameter, Tvalue value, Args... args)
    {
      doc[parameter]=value;
      assembleJSON(doc, args...);
    }
    // Low level EEPROM storage functions:
    bool read_NVM(DynamicJsonDocument& doc);
    void write_NVM(DynamicJsonDocument& doc);

  public:
      Json_NVM(unsigned int _NVM_offset, unsigned int _NVM_size);
      unsigned int size();
      bool readJSON(DynamicJsonDocument& doc);
      void dump_NVM();
      void dump_NVM(unsigned int _NVM_offset, unsigned int _NVM_size);

      // Template functions to write s variable number of input parameters as JSON object:
      template<typename... Args>
      void writeJSON(Args... args)
      {
          DynamicJsonDocument doc(NVM_size);
          assembleJSON(doc, args...);
          write_NVM(doc);
      }
};
