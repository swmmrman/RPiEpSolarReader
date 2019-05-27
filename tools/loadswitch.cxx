#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <modbus-rtu.h>
#include <climits>
#include <ctime>
#include <iomanip>

using namespace std;

int main(int argc, char *argv[]) {
    modbus_t *ctx;
    int errno;
    int rc;
    int rd = 0;
    int loadReg;
    struct timeval *timeout =  (timeval*)malloc(sizeof(timeval));
    timeout->tv_sec = 3;
    timeout->tv_usec = 0;
    
    char *newloadStatus = (char *)malloc(3);
    
    loadReg = 0x2;
    ctx = modbus_new_rtu("/dev/ttyUSB0", 115200, 'N', 8, 1);
    modbus_flush(ctx);
    modbus_set_response_timeout(ctx, timeout);
    uint8_t tab_bits[3];
    uint8_t loadstatus;
    if(modbus_connect(ctx) <= -1) {
        cout << "Could not connect\n";
        goto exit;
    }
    rc = modbus_set_slave(ctx, 1);
    if(rc == -1) {
        cout << "Unable to set slave id: " << modbus_strerror(errno) << endl;
        goto exit;
    }
    rc = -1;
    while(rc == -1) {
        rc = modbus_read_bits(ctx, loadReg, 1, tab_bits);
    }
    loadstatus = tab_bits[0];
    rd = modbus_write_bit(ctx, loadReg, loadstatus==1?0:1);
    if(rd == -1) {
        cout << "Setting gave: " << modbus_strerror(errno) << endl;
        //goto exit;
    }
    rc = -1;
    while(rc == -1) {
        rc = modbus_read_bits(ctx, loadReg, 1, tab_bits);
    }
    strcpy(newloadStatus,loadstatus==0?"On":"Off");
    if(loadstatus != tab_bits[0]) {
        cout << "Load changed to " << newloadStatus << ".\n";
    }
    else {
        cout << "Load did not change";
    }
    modbus_close(ctx);
    modbus_free(ctx);
    
    return 0;

    exit:
        modbus_close(ctx);
        modbus_free(ctx);
        return 1;
    
}
