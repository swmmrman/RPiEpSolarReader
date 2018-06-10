#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <modbus-rtu.h>

using namespace std;

int main(int argc, char **argv) {
  modbus_t *mb;
  int registers = 24;
  uint8_t tab_reg[registers];

  mb = modbus_new_rtu("/dev/ttyXRUSB0", 115200, 'N', 8, 1);
  modbus_connect(mb);
  modbus_set_slave(mb, 2);
  /* Read 5 registers from the address 0 */
  modbus_read_bits(mb, 0x12544,registers, tab_reg);
  int i;
  for(i=0; i<registers; i++) {
    fflush(stdout);
    float val = tab_reg[i];
    printf("Register %d: %d\n", i, tab_reg[i]);
  }
  modbus_close(mb);
  modbus_free(mb);
}

