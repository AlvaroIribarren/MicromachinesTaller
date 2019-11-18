#ifndef _PLUGIN_H
#define _PLUGIN_H

extern int unique_signal; // for use in proving symbol stuff.

#include <string>

class Plugin {

public:
    virtual std::string message(void) = 0;

};

#endif

/*#include <stdio.h>
#include "DTOs.h"

int plugin_func(void);

void plugin_car_damaged(CarDTO_t* cars[], size_t size);

*/