#ifndef LIB_H
#define LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#pragma region BOOL-SUPPORT
typedef short int bool;
const bool false = 0;
const bool true = !false;
#define TRUE true
#define FALSE false
#pragma endregion

#endif