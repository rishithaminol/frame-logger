#include <stdio.h>

#include "module.h"

static int return_something_internal()
{
    printf("Internal function called int will return\n");
    
    return 666;
}

void call_internal_function()
{
    printf("Calling internal function\n");
    int x = return_something_internal();

    print("Internal function value = %d\n", x);
}
