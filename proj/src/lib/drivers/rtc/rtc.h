
#ifndef _PROJ_RTC_H
#define _PROJ_RTC_H

#include <lcom/lcf.h>

#define IRQ_RTC             8

#define RTC_ADDR_REG        0x70
#define RTC_DATA_REG        0x71

#define RTC_DM              BIT(2)
#define RTC_UIP             BIT(7)

#define RTC_SECONDS         0
#define RTC_SECONDS_ALARM   1
#define RTC_MINUTES         2
#define RTC_MINUTES_ALARM   3
#define RTC_HOURS           4
#define RTC_DAY             7
#define RTC_MONTH           8
#define RTC_YEAR            9
#define RTC_REGISTER_A      10
#define RTC_REGISTER_B      11
#define RTC_REGISTER_C      12
#define RTC_REGISTER_D      13

typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
} rtc_info_t;
rtc_info_t rtcInfo;

int rtc_init();
int rtc_subscribe_int();
int rtc_unsubscribe_int();
int rtcIN(uint8_t command, uint8_t *output);
int rtcOUT(uint8_t command, uint8_t output);
bool rtcIsUpdating();
int rtcUpdate();
uint8_t _bcdToBinary(uint8_t bcd);

#endif // _PROJ_RTC_H
