#ifndef ETHERTYPE_H
#define ETHERTYPE_H

#include <sys/types.h>

typedef struct fl_eth_type {
    u_int16_t   type_value;
    char        *type_desc;
    char        short_code[256];
    int         range;             /* This will become 1, If type_value is a range  */
    u_int16_t   type_value_start;
    u_int16_t   type_value_end;
} fl_eth_type_t;

extern fl_eth_type_t *get_fl_eth_type(u_int16_t eth_type_);

#endif
