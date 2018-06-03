#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <modbus-rtu.h>
#include <climits>

int DEBUG = 0;

using namespace std;

int main(int argc, char *argv[]) {
  modbus_t *ctx;
  int registers = 15;
  int startReg = 12544;
  int errno;
  int rc;
  uint16_t tab_bits[registers];

  if(argc < 2) {
    cout << "Usage: " << argv[0] << "<path-to-device>\n";
    return 1;
  }
  if(argc > 2) {
    registers = stoi(argv[2]);
  }
  ctx = modbus_new_rtu(argv[1], 115200, 'N', 8, 1);
  if(DEBUG) {
	  cout << "Device: " << argv[1] << "\n";
  }
  if(modbus_connect(ctx) <= -1) {
	  cout << "Could not connect\n";
	  goto exit;
  } else {
	  cout << "Connection succeeded\n";
  }
  if(DEBUG) modbus_set_debug(ctx, TRUE);
  rc = modbus_set_slave(ctx, 1);
  if(rc == -1) {
	  cout << "Unable to set slave id: " << modbus_strerror(errno) << endl;
	  goto exit;
  }
  int length;
  length = modbus_read_input_registers(ctx, startReg, registers,tab_bits);
  if(length == -1) {
    cout << modbus_strerror(errno) << endl;
    goto exit;
  }
  int i;

  cout << length << " bits read\n";
  for(i=0; i<length;i++) {
    fflush(stdout);
    printf("Register %d: %d \n", startReg+i, tab_bits[i]);
  }
  modbus_close(ctx);
  modbus_free(ctx);
  return 0;

  exit:
  modbus_close(ctx);
	modbus_free(ctx);
	return 1;
}
