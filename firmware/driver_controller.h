#ifndef DRIVER_CONTROLLER_H
#define DRIVER_CONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "dd_types.h"

void initCtrDrv(void);
char initDriver(char driverID);
char callDriver(char driverID, char command, void * args);

#ifdef __cplusplus
}
#endif

#endif
