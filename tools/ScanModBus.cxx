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
  int validSlaves[1024];
  int slaveCounter;
  int *slave;
  int usedSlaves[1024];
  int usedSlaveCounter;
  int rc;
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
  slaveCounter = 0;
  usedSlaveCounter = 0;
  for(int i=0; i<1024;i++) {
    cout << "testing slave id: " << i;
    rc = modbus_set_slave(ctx, i);
    if(rc == -1) {
      continue;
      cout << " Invalid" << endl;
    }
    else {
      cout << " valid id" << endl;
      int length;
      for(int j=0; j<10; j++) {
        length = modbus_read_input_registers(ctx, 0x3100, 1, tab_bits);
        if(length > 0) {
          cout << " Recieved " << length << "bits. Marked Valid" << endl;
          validSlaves[slaveCounter] = i;
          slaveCounter++;
          break;
        }
        else {
          if(errno == 2) {
            usedSlaves[usedSlaveCounter];
            usedSlaveCounter++;
            cout << "Slave in use" << endl;
            break;
          }
          else if(errno == 112345690) {
            cout << "CRC error trying again." << endl;
            sleep(1);
            j--;
            errno = 0;
            continue;
          }
          cout << " Try " << j << " " << modbus_strerror(errno) << " Err:" <<
               errno;
        }
        cout << endl;
      }
    }
  }
  slave = validSlaves;
  cout << *slave;
  slave++;
  for(;slave<&validSlaves[1024];slave++) {
    cout << "," << *slave;
  }

  /*int length;
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
  }*/
  modbus_close(ctx);
  modbus_free(ctx);
  return 0;

  exit:
  modbus_close(ctx);
	modbus_free(ctx);
	return 1;
}
