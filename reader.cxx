#include <cstdlib>
#include <iostream>
#include <modbus-rtu.h>
#include <climits>

int DEBUG = 0;

using namespace std;

int main(int argc, char *argv[]) {
  modbus_t *ctx;
  int errno;
  if(argc < 2) {
    cout << "usage" << argv[0] << "<path-to-device>\n";
    return 1;
  }
  ctx = modbus_new_rtu(argv[1], 115200, 'N', 8, 1);
  if(DEBUG) {
	  cout << "Device: " << argv[1] << "\n";
  }
  if(modbus_connect(ctx) == -1) {
	  cout << "Could not connect\n";
	  goto exit;
  } else {
	  cout << "Connection succeeded\n";
	  modbus_free(ctx);
  }
  /*Not needed if USB->RS485 See https://github.com/stephane/libmodbus/issues/316
  res = modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
  if(res == -1) {
	  cout << "Failed to set serial mode: " << modbus_strerror(errno) << 
			"\n";
	  goto exit;	
  }*/
  if(modbus_set_slave(ctx, 1) == -1) {
	  cout << "Unable to set slave id: " << modbus_strerror(errno) << 
			"\n";
	  goto exit;
  }

  return 0;
  exit: 
	modbus_free(ctx);
	return 1;
}


