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
  string names[] = { "Array V", "Array A", "Array W", "", "Bat V",
					     "Bat Amps", "Bat Watts", "", "", "", "",
						 "", "Load V", "Load Amps", "Load Watts"}; 
  string labels[] = { "volts", "amps", "watts", "", "volts", "amps", 
				      "watts", "", "", "", "", "", "volts", "amps", 
				      "watts"};
  string categories[] = {"\n<----Array Values---->\n", 
	                     "\n<----Battery Values---->\n", 
						 "\n<----Load Values---->\n"};			 
  int j=1;
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
  cout << categories[0];
  for(i=0; i<length;i++) {
    //fflush(stdout);
    //printf("Register %d: %d \n", startReg+i, tab_bits[i]);
	if(names[i] == "") {
		if(j < 3) cout << categories[j];
		j++;
		continue;
	}
    if(DEBUG) cout << "Register: " << hex << startReg+i;
    cout << " Name: " << names[i];
    cout << " Value: " << dec << ((float)tab_bits[i]/100) << labels[i];
	cout << endl;
  }
  length = -1;
  j = 0;
  while(length == -1) {
	  length = modbus_read_input_registers(ctx, 0x330C, 1, tab_bits);
	  if(j == 5) break;
	  j++;
  }
  cout << "\n<----Daily Stats---->\n";
  if(DEBUG) cout << "Register 0x330C: ";
  cout << "Energy Generated: " << dec << (tab_bits[0]/100) << " KWH\n";
  length = -1;
  j = 0;
  while(length == -1) {
	  length = modbus_read_input_registers(ctx, 0x3304, 1, tab_bits);
	  if(j == 5) break;
	  j++;
  }
  cout << "Energy Used: " << dec << (tab_bits[0]/100) << "KWH\n";
  modbus_close(ctx);
  modbus_free(ctx);
  return 0;

  exit:
  modbus_close(ctx);
	modbus_free(ctx);
	return 1;
}
