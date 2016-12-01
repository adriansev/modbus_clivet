#include <stdio.h>
#include "modbus/modbus-rtu.h"
#include "modbus/modbus-tcp.h"
#include "modbus/modbus-version.h"
#include "modbus/modbus.h"


int main () {
modbus_t *mb;
uint16_t tab_reg[32];

mb = modbus_new_tcp("10.10.8.27", 502);
modbus_connect(mb);

/* Read 5 registers from the address 0 */
modbus_read_registers(mb, 0, 5, tab_reg);

modbus_close(mb);
modbus_free(mb);


return 0;
}



