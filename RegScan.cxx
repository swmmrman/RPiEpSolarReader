#include <cstdint>
#include <string>
#include <cstdlib>
#include <iostream>
#include <modbus-rtu.h>
#include <climits>
#include <unistd.h>

int DEBUG = 0;

using namespace std;

int main(int argc, char *argv[]) {
  modbus_t *ctx;
  uint16_t tab_bits[2];
  char* dev = (char*)"/dev/ttyXRUSB0";
  int rc;
  int slaveid = 3; //1 and 2 seem to be taken by the Elog-01 and MT-50
  int validReg[0x90ff];
  if(argc < 2) {
    cout << "No device specified, using " << dev << endl;
  }
  else {
    dev = argv[1];
  }
  ctx = modbus_new_rtu(dev, 115200, 'N', 8, 1);
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
  rc = modbus_set_slave(ctx, slaveid);
  if(rc == -1) {
    cout << "Slave id was not valid.";
    goto exit;
  }
  for(int i=0x00; i<=0x90ff; i++){
    int length;
    int regCounter = 0;
    length = modbus_read_input_registers(ctx, i, 1, tab_bits);
    if(length > 0){
      cout << "Found valid register at: " << hex << i << " Value: " <<
              tab_bits[0] << " Nytes read: " << length << endl;
      validReg[regCounter] = i;
      regCounter++;
    }
    //cout << uppercase <<hex << i << endl;
  }

  modbus_close(ctx);
  modbus_free(ctx);
  return 0;

  exit:
  modbus_close(ctx);
	modbus_free(ctx);
	return 1;
}
