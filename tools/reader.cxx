#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <modbus-rtu.h>
#include <climits>
#include <ctime>
#include <iomanip>

int DEBUG = 0;

using namespace std;

int main(int argc, char *argv[]) {
  modbus_t *ctx;
  int registers = 15;
  int startReg = 12544;
  int errno;
  int rc;
  timeval *timeout;
  timeout->tv_sec = 3;
  timeout->tv_usec = 0;
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
  modbus_flush(ctx);
  modbus_set_response_timeout(ctx, timeout);
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
    if(DEBUG) cout << "Register: " << dec << startReg+i;
    cout << " Name: " << names[i];
    cout << " Value: " << dec << ((float)tab_bits[i]/100) << " ";
	cout << labels[i] << endl;
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
  cout << "Energy Generated: " << dec << ((float)tab_bits[0]/100) << " KWH\n";
  length = -1;
  j = 0;
  while(length == -1) {
	  length = modbus_read_input_registers(ctx, 0x3304, 1, tab_bits);
	  if(j == 5) break;
	  j++;
  }
  cout << "Energy Used: " << dec << ((float)tab_bits[0]/100) << " KWH\n";
  cout << "\n<----Monthly Stats----> \n";
  j = 0;
  length = -1;
  while(length == -1) {
    length = modbus_read_input_registers(ctx, 0x330E, 1, tab_bits);
    if(j == 5) break;
    j++;
  }
  if(DEBUG) cout << "Register Ox330E: ";
  cout << "Energy Generated: " << dec << ((float)tab_bits[0]/100) << "KWH\n";
  length = -1;
  j = 0;
  while(length == -1) {
    length = modbus_read_input_registers(ctx, 0x3306, 1, tab_bits);
    if(j == 5) break;
    j++;
  }
  if(DEBUG) cout << "Register 0x3306: ";
  cout << "Energy Used: " << dec << ((float)tab_bits[0]/100) << "KWH\n";
  length = -1;
  //9015 = high byte year   /low byte month
  //9014 = high byte day    /low byte hour
  //9013 = high byte minute /low byte second
  while(length == -1) {
    length = modbus_read_registers(ctx, 0x9013, 3, tab_bits);
    if(j == 5) break;
    j++;
  }
  unsigned int year, month, day, hour, minute, second;
  year = ((tab_bits[2] >> 8) & 0xff) + 2000;
  month = tab_bits[2] & 0xff;
  day = (tab_bits[1] >> 8) & 0xff;
  hour = tab_bits[1] & 0xff;
  minute = (tab_bits[0] >> 8) & 0xff;
  second = tab_bits[0] & 0xff;
  cout << "Time: " << year << "/";
  cout << setfill('0') << setw(2) << month << "/";
  cout << setfill('0') << setw(2) << day;
  cout << " " << setfill('0') << setw(2) << hour << ":";
  cout << setfill('0') << setw(2) << minute << ":"; 
  cout << setfill('0') << setw(2) << second << "\n";
  modbus_close(ctx);
  modbus_free(ctx);
  return 0;

  exit:
  modbus_close(ctx);
	modbus_free(ctx);
	return 1;
}
