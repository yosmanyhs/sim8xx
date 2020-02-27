#ifndef TEST_{{upperCase name}}_STATIC_H
#define TEST_{{upperCase name}}_STATIC_H

#include "AtCommand.h"

size_t {{pascalCase name}}Serialize(void *obj, char *obuf, size_t length);
size_t {{pascalCase name}}Parse(void *obj, const char *ibuf, size_t length);
void {{pascalCase name}}Timeout(void *p);

#endif