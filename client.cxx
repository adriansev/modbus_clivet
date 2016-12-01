/*
 *  NOTA BENE !!!!!!!!!!!
 *  This is a C++ client with shameless usage of the C libmodbus libraries
 */

// #include <stdio.h>
#include <iostream>
#include <cstdio>
#include <cerrno>
#include <cstring>

#include "modbus/modbus-rtu.h"
#include "modbus/modbus-tcp.h"
#include "modbus/modbus-version.h"
#include "modbus/modbus.h"

#define MIXED_DATA_ADDR 500
#define SOFTWARE_VER_ADDR 1

// REFERENCE DOCUMENT
// http://monitor.spacescience.ro/hardware/CLIVET/WSAT-XEE.pdf

int main () {
modbus_t *mb = NULL;

uint16_t mixed_data_tbl [34];
std::memset(mixed_data_tbl, 0, sizeof mixed_data_tbl); // set the content to 0

uint16_t software_ver_tbl [4];
std::memset(software_ver_tbl, 0, sizeof software_ver_tbl); // set the content to 0

int rc;

mb = modbus_new_tcp("10.10.8.27", 502);
if (modbus_connect(mb) == -1) {
    std::cerr << "Connection failed: " << modbus_strerror(errno) << std::endl;
    modbus_free(mb);
    return -1;
    }

// Read the software version table
// info at page 5, begining of the page
rc = modbus_read_registers(mb, SOFTWARE_VER_ADDR, sizeof(software_ver_tbl)/sizeof(software_ver_tbl[0]), software_ver_tbl);
if (rc == -1) {
    std::cerr << "Reading registers : " << modbus_strerror(errno) << std::endl;
    modbus_free(mb);
    return -1;
    }

uint16_t firm_screen = software_ver_tbl[0];
uint16_t firm_rev = software_ver_tbl[1];
uint8_t firm_rev_e2 = MODBUS_GET_HIGH_BYTE(software_ver_tbl[2]);

std::cout << "Firmware screen = " << firm_screen << std::endl;
std::cout << "Firmware revision = " << firm_rev  << std::endl;
std::cout << "Firmware revision E2 = " << firm_rev_e2  << std::endl;

// Read the full size of table starting with MIXED_DATA_ADDR
// info at page 12, end of the page table
rc = modbus_read_registers(mb, MIXED_DATA_ADDR, (int)(sizeof(mixed_data_tbl)/sizeof(mixed_data_tbl[0])), mixed_data_tbl);
if (rc == -1) {
    std::cerr << "Reading registers : " << modbus_strerror(errno) << std::endl;
    modbus_free(mb);
    return -1;
    }

float temp_return = mixed_data_tbl[3];
float temp_supply = mixed_data_tbl[4];
float temp_freshair = mixed_data_tbl[5];

// std::cout << "Return temp = " << ((float)temp_return)/10 << std::endl;
// std::cout << "Supply temp = " << ((float)temp_supply)/10 << std::endl;
// std::cout << "Fresh air temp = " << ((float)temp_freshair)/10 << std::endl;

std::cout << "Return temp = " << temp_return << std::endl;
std::cout << "Supply temp = " << temp_supply << std::endl;
std::cout << "Fresh air temp = " << temp_freshair << std::endl;


for (int i=0; i < rc; i++) {
    printf("reg[%d]=%d (0x%X)\n", i, mixed_data_tbl[i], mixed_data_tbl[i]);
    }


modbus_close(mb);
modbus_free(mb);


return 0;
}



