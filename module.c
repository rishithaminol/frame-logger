#include <stdio.h>

#include "module.h"

/**
 * @brief This function is internal function. No one can see it
*/
static int return_something_internal(void)
{
    printf("Internal function called int will return\n");
    
    return 666;
}

void call_internal_function(void)
{
    printf("Calling internal function\n");
    int x = return_something_internal();

    printf("Internal function value = %d\n", x);
}
