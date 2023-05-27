#include <lcom/lcf.h>
#include <lcom/utils.h>
#include "i8042.h"
#include "KBC.h"

int readKBCStatus(uint8_t* status){
    return util_sys_inb(KBC_STATUS_REG, status);
}

int readKBCOutput(uint8_t port, uint8_t *output, bool mouse){
    uint8_t status;
    uint8_t attempts = MAX_ATTEMPTS_KBC;

    while (attempts--){
        if (readKBCStatus(&status)) {
            printf("(readKBCOutput) readKBCStatus failed.\n");
            return 1;
        }
        if (status & FULL_OUT_BUFFER) {
            if (status & PARITY_ERROR) {
                printf("KBC Parity Error occured.\n");
                return 1;
            }
            if (status & TIMEOUT_ERROR) {
                printf("KBC Timeout Error occured.\n");
                return 1;
            }
            if (mouse && !(status & MOUSE_BIT)) {
                printf("readKBCOutput expected mouse but keyboard info found.\n");
                return 1;
            }
            if (!mouse && (status & MOUSE_BIT)) {
                printf("readKBCOutput expected keyboard but mouse info found.\n");
                return 1;
            }
            return util_sys_inb(port, output);
        }
        tickdelay(micros_to_ticks(WAIT_KBC));
    }
    printf("readKBCOutput attempted %d times but failed.\n", MAX_ATTEMPTS_KBC);
    return 1;
}

int writeKBCCommand(uint8_t port, uint8_t commandByte){
    uint8_t status;
    uint8_t attempts = MAX_ATTEMPTS_KBC;

    while (attempts--) {
        if (readKBCStatus(&status)) {
            printf("(writeKBCCommand) readKBCStatus failed.\n");
            return 1;
        }
        if (!(status & FULL_IN_BUFFER)) { // ATIVADO A ZERO
            if (sys_outb(port, commandByte)) return 1;
            return 0;
        }
        tickdelay(micros_to_ticks(WAIT_KBC));
    }
    printf("writeKBCCommand attempted %d times but failed.\n", MAX_ATTEMPTS_KBC);
    return 1;
}
