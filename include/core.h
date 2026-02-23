#ifndef INCLUDE_CORE_H_
#define INCLUDE_CORE_H_

#include "./vector.hpp"


void core_delete_vector(Types type, void* vectorPtr);
void core_print_vector(Types type, void* &vectorPtr);
void core_input_vector(Types type, void* &vectorPtr);

void input_alias(char*alias);
Types input_type(Types type, void* &vectorPtr);

void mainLoop();
#endif  // INCLUDE_CORE_H_
