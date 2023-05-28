
#include "rtc.h"

int hookIdRTC = 5;
bool isRtcBinary = false;

int rtc_init() {
    uint8_t regB;
    if (rtcIN(RTC_REGISTER_B, &regB)) return 1;
    isRtcBinary = (regB & RTC_DM) ? true : false;
    return rtcUpdate();
}

int rtc_subscribe_int() {
    return sys_irqsetpolicy(IRQ_RTC, IRQ_REENABLE, &hookIdRTC);
}

int rtc_unsubscribe_int() {
    return sys_irqrmpolicy(&hookIdRTC);
}

int rtcIN(uint8_t command, uint8_t *output) {
    if (sys_outb(RTC_ADDR_REG, command)) {
        printf("(rtcIN) Failed writing to RTC_ADDR_REG.\n");
        return 1;
    }
	if (util_sys_inb(RTC_DATA_REG, output)) {
        printf("(rtcIN) Failed reading from RTC_DATA_REG.\n");
        return 1;
    }
    return 0;
}

int rtcOUT(uint8_t command, uint8_t output) {
    if (sys_outb(RTC_ADDR_REG, command)) {
        printf("(rtcOUT) Failed writing to RTC_ADDR_REG.\n");
        return 1;
    }
	if (sys_outb(RTC_DATA_REG, output)) {
        printf("(rtcOUT) Failed writing from RTC_DATA_REG.\n");
        return 1;
    }
    return 0;
}

bool rtcIsUpdating() {
    uint8_t updating;
    if (rtcIN(RTC_REGISTER_A, &updating)) return 1;
	return (bool) (updating & RTC_UIP);
}

int rtcUpdate() {
    // If is updating, exit
    if (rtcIsUpdating()) return 1;

    uint8_t output;
    if (rtcIN(RTC_SECONDS, &output)) return 1;
    rtcInfo.second = isRtcBinary ? output : _bcdToBinary(output);
    if (rtcIN(RTC_MINUTES, &output)) return 1;
    rtcInfo.minute = isRtcBinary ? output : _bcdToBinary(output);
    if (rtcIN(  RTC_HOURS, &output)) return 1;
    rtcInfo.hour   = isRtcBinary ? output : _bcdToBinary(output);
    if (rtcIN(    RTC_DAY, &output)) return 1;
    rtcInfo.day    = isRtcBinary ? output : _bcdToBinary(output);
    if (rtcIN(  RTC_MONTH, &output)) return 1;
    rtcInfo.month  = isRtcBinary ? output : _bcdToBinary(output);
    if (rtcIN(   RTC_YEAR, &output)) return 1;
    rtcInfo.year   = isRtcBinary ? output : _bcdToBinary(output);

    return 0;
}

uint8_t _bcdToBinary(uint8_t bcd) {
    return  (bcd & 0xF) + ((bcd >> 4) * 10);
}
