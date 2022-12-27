#ifndef JSON_LIB_H
#define JSON_LIB_H
#include "general_include.h"
#include <cJSON.h>
#include <cJSON_os.h>


// Prototypes
int json_add_obj(cJSON *parent, const char *str, cJSON *item);
int json_add_str(cJSON *parent, const char *str, const char *item);
int json_add_number(cJSON *parent, const char *str, double item);

#endif /* JSON_LIB_H */