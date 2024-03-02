#ifndef __LOG_H__ //header guard
#define __LOG_H__

#include<stdio.h>

#define GREEN   "\x1b[32m"
#define RED     "\x1b[31m"
#define YELLOW  "\x1b[33m"
#define RESET   "\x1b[0m"
#define BLUE    "\x1b[34m"

void LogSuccess(const char* message) {
    printf(GREEN "[+] %s" RESET "\n", message);
}

void LogError(const char* message) {
    printf(RED "[-] %s" RESET "\n", message);
}

void LogWarning(const char* message) {
    printf(YELLOW "[!] %s" RESET "\n", message);
}

void LogInfo(const char* message) {
    printf(BLUE "[?] %s" RESET "\n", message);
}

#endif //LOG_H