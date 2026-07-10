#pragma once

#ifndef MEMORY_H
#define MEMORY_H

#include <string>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdint.h>

// Initialize memory access to the target process
bool InitializeMemoryAccess();

// Memory read/write function templates
template<typename T>
T ReadMemory(pid_t pid, long address);

template<typename T>
T ReadMemory(uintptr_t address); //Overload thats to be used

void ReadMemoryBuffer(pid_t pid, long address, void* buffer, size_t size);
void ReadMemoryBuffer(uintptr_t address, void* buffer, size_t size); //Overload thats to be used

template<typename T>
bool WriteMemory(pid_t pid, long address, const T& value);

std::string ReadFString(pid_t pid, long fstringAddress);
std::string ReadFString (uintptr_t address);

pid_t FindGamePID();

// Global variables
extern pid_t ProcessId;
extern long BaseAddress;


#endif //MEMORY_H