
#include "serialPort.h"

int hookIdSerialPort = 3;
uint8_t receivingSerialPort = INFO_ACK;
uint8_t indexSerialPort = 0;
uint8_t bitsSerialPort[8] = {0};
bool waitingAckSerialPort = false;

int serial_port_subscribe_int() {
    irqSetSerialPort = BIT(hookIdSerialPort);
    return sys_irqsetpolicy(COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookIdSerialPort);
}

int serial_port_unsubscribe_int() {
    return sys_irqrmpolicy(&hookIdSerialPort);
}

int serial_port_init(){
    uint8_t ier;
    if (util_sys_inb(IER, &ier)) return 1;
    ier &= IER_CLEAR;
    return sys_outb(IER, ier | IER_ENABLE_RECEIVED_INT);
}

int serial_port_get_status(uint8_t* status) {
    return util_sys_inb(LSR, status);
}

int serial_port_ih() {
    uint8_t iir;
    if (util_sys_inb(IIR, &iir)) return 1;
    if (iir & IIR_NO_INTERRUPT_PENDING) {
        printf("No interrupt pending.\n");
        return 0;
    }
    if ((iir & IIR_INTERRUPT_PENDING_INFO) != IIR_RECEIVED_DATA_AVAILABLE) {
        printf("No data.\n");
        return 0;
    }
    int ret = serialPortReadByte() ? 0 : 1;
    switch (serialPortByte) {
        case INFO_ULONG: case INFO_UINT: case INFO_FLOAT:
        case INFO_LONG:  case INFO_INT:  case INFO_DOUBLE:
            receivingSerialPort = serialPortByte;
            return 0;
        default: break;
    }
    return ret;
}

int serialPortSendByte(uint8_t byte) {
    uint8_t status;
    uint8_t attempts = MAX_ATTEMPTS_UART;
    while (attempts--) {
        if (serial_port_get_status(&status)) {
            printf("(serialPortSendByte) serial_port_get_status failed.\n");
            return 1;
        }
        if (status & LSR_THR_EMPTY) {
            if (sys_outb(THR, byte)) {
                printf("Failed to write Data to THR.\n");
                return 1;
            } else return 0;
        }
        tickdelay(micros_to_ticks(WAIT_UART));
    }
    printf("serialPortSendByte attempted %d times but failed.\n", MAX_ATTEMPTS_UART);
    return 1;
}

int serialPortReadByte() {
    uint8_t status;
    if (serial_port_get_status(&status)) {
        printf("(serialPortReadByte) serial_port_get_status failed.\n");
        return 1;
    }
    if (status & LSR_OVERRUN_ERROR) {
        printf("UART Overrun Error occured.\n");
        return 1;
    }
    if (status & LSR_PARITY_ERROR) {
        printf("UART Parity Error occured.\n");
        return 1;
    }
    if (status & LSR_FRAMING_ERROR) {
        printf("UART Framing Error occured.\n");
        return 1;
    }
    if (!(status & LSR_DATA_READY)) {
        printf("RBR Data isn't ready.\n");
        return 1;
    }
    uint8_t rbr;
    if (util_sys_inb(RBR, &rbr)) {
        printf("Failed to read RBR Data.\n");
        return 1;
    }
    serialPortByte = rbr;
    return 0;
}

int serialPortSendULong(uint64_t longValue) {
    if (serialPortSendByte(INFO_ULONG)) return 1;
    uint8_t *bits = (uint8_t *) &longValue;
    for (int i = 0; i < 8; i++)  if (serialPortSendByte(bits[i])) return 1;
    waitingAckStartTime = timer_get_elapsed_count();
    waitingAckSerialPort = true;
    return 0;
}

int serialPortSendLong(int64_t longValue) {
    if (serialPortSendByte(INFO_LONG)) return 1;
    uint8_t *bits = (uint8_t *) &longValue;
    for (int i = 0; i < 8; i++) if (serialPortSendByte(bits[i])) return 1;
    waitingAckStartTime = timer_get_elapsed_count();
    waitingAckSerialPort = true;
    return 0;
}

int serialPortSendUInt(uint32_t intValue) {
    if (serialPortSendByte(INFO_UINT)) return 1;
    uint8_t *bits = (uint8_t *) &intValue;
    for (int i = 0; i < 4; i++) if (serialPortSendByte(bits[i])) return 1;
    waitingAckStartTime = timer_get_elapsed_count();
    waitingAckSerialPort = true;
    return 0;
}

int serialPortSendInt(int32_t intValue) {
    if (serialPortSendByte(INFO_INT)) return 1;
    uint8_t *bits = (uint8_t *) &intValue;
    for (int i = 0; i < 4; i++) if (serialPortSendByte(bits[i])) return 1;
    waitingAckStartTime = timer_get_elapsed_count();
    waitingAckSerialPort = true;
    return 0;
}

int serialPortSendFloat(float floatValue) {
    if (serialPortSendByte(INFO_FLOAT)) return 1;
    uint8_t *bits = (uint8_t *) &floatValue;
    for (int i = 0; i < 4; i++) if (serialPortSendByte(bits[i])) return 1;
    waitingAckStartTime = timer_get_elapsed_count();
    waitingAckSerialPort = true;
    return 0;
}

int serialPortSendDouble(double doubleValue) {
    if (serialPortSendByte(INFO_DOUBLE)) return 1;
    uint8_t *bits = (uint8_t *) &doubleValue;
    for (int i = 0; i < 8; i++) if (serialPortSendByte(bits[i])) return 1;
    waitingAckStartTime = timer_get_elapsed_count();
    waitingAckSerialPort = true;
    return 0;
}

int serialPortReadULong(uint64_t* longValue) {
    if (serialPortReadByte()) return 1;
    bitsSerialPort[indexSerialPort++] = serialPortByte;
    if (indexSerialPort < 8) return 1;
    *longValue = *((uint64_t *) (bitsSerialPort));
    receivingSerialPort = INFO_ACK;
    indexSerialPort = 0;
    return 0;
}

int serialPortReadLong(int64_t* longValue) {
    if (serialPortReadByte()) return 1;
    bitsSerialPort[indexSerialPort++] = serialPortByte;
    if (indexSerialPort < 8) return 1;
    *longValue = *((int64_t *) (bitsSerialPort));
    receivingSerialPort = INFO_ACK;
    indexSerialPort = 0;
    return 0;
}

int serialPortReadUInt(uint32_t* intValue) {
    if (serialPortReadByte()) return 1;
    bitsSerialPort[indexSerialPort++] = serialPortByte;
    if (indexSerialPort < 4) return 1;
    *intValue = *((uint32_t *) (bitsSerialPort));
    receivingSerialPort = INFO_ACK;
    indexSerialPort = 0;
    return 0;
}

int serialPortReadInt(int32_t* intValue) {
    if (serialPortReadByte()) return 1;
    bitsSerialPort[indexSerialPort++] = serialPortByte;
    if (indexSerialPort < 4) return 1;
    *intValue = *((int32_t *) (bitsSerialPort));
    receivingSerialPort = INFO_ACK;
    indexSerialPort = 0;
    return 0;
}

int serialPortReadFloat(float* floatValue) {
    if (serialPortReadByte()) return 1;
    bitsSerialPort[indexSerialPort++] = serialPortByte;
    if (indexSerialPort < 4) return 1;
    *floatValue = *((float *) (bitsSerialPort));
    receivingSerialPort = INFO_ACK;
    indexSerialPort = 0;
    return 0;
}

int serialPortReadDouble(double* doubleValue) {
    if (serialPortReadByte()) return 1;
    bitsSerialPort[indexSerialPort++] = serialPortByte;
    if (indexSerialPort < 8) return 1;
    *doubleValue = *((double *) (bitsSerialPort));
    receivingSerialPort = INFO_ACK;
    indexSerialPort = 0;
    return 0;
}

int serialPortSendAck() {
    serialPortClearFIFO();
    return serialPortSendByte(INFO_ACK);
}

int serialPortSendReady() {
    waitingAckSerialPort = true;
    return serialPortSendByte(INFO_READY);
}

int serialPortSendNReady() {
    waitingAckSerialPort = true;
    return serialPortSendByte(INFO_NREADY);
}

int serialPortSendReadyAck() {
    serialPortClearFIFO();
    return serialPortSendByte(INFO_READY_ACK);
}

int serialPortSendNReadyAck() {
    serialPortClearFIFO();
    return serialPortSendByte(INFO_NREADY_ACK);
}

int serialPortSendWon() {
    return serialPortSendByte(INFO_WON);
}

int serialPortSendExit() {
    return serialPortSendByte(INFO_EXIT);
}

int serialPortSendNack() {
    return serialPortSendByte(INFO_NACK);
}

int serialPortClearFIFO() {
    return sys_outb(FCR, FCR_CLEAR);
}
