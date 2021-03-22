#pragma once

#include <stdio.h>

#include <iostream>

void* operator new(std::size_t size, const char* file, int line);
void* operator new[](std::size_t size, const char* file, int line);

#define new new (__FILE__, __LINE__)

int checkLeaks();
