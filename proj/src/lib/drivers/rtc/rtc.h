
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
/**
 * @brief Structure to hold RTC (Real-Time Clock) information.
 */
typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
} rtc_info_t;
rtc_info_t rtcInfo;
/**
 * @brief Initializes the RTC (Real-Time Clock).
 *
 * This function initializes the RTC by reading the value of register B and setting the isRtcBinary variable based on the value of the RTC_DM bit in register B.
 * @return Non-zero if an error occured.
 */
int rtc_init();
/**
 * @brief Subscribes to RTC interrupts.
 *
 * This function subscribes to RTC interrupts by calling sys_irqsetpolicy with the IRQ_RTC parameter, IRQ_REENABLE flag, and the address of the hookIdRTC variable.
 *
 * @return Return value of sys_irqsetpolicy. Returns a negative value if an error occurs, otherwise returns the bit mask representing the IRQ line.
 */
int rtc_subscribe_int();
/**
 * @brief Unsubscribes from RTC interrupts.
 *
 * This function unsubscribes from RTC interrupts by calling sys_irqrmpolicy with the address of the hookIdRTC variable.
 * @return Return value of sys_irqrmpolicy. Returns non-zero value if an error occurs.
 */
int rtc_unsubscribe_int();
/**
 * @brief Reads data from RTC.
 *
 * This function reads data from the RTC by writing the specified command to RTC_ADDR_REG and then reading the data from RTC_DATA_REG.
 * @param command command to write to RTC_ADDR_REG.
 * @param output pointer to a variable to store the read data.
 * @return Non-zero if an error occured.
 */
int rtcIN(uint8_t command, uint8_t *output);
/**
 * @brief Checks if RTC is updating.
 *
 * This function checks if the RTC is currently in the updating state by reading the RTC_REGISTER_A and checking the RTC_UIP flag.
 * @return True if the RTC is updating, False otherwise.
 */
bool rtcIsUpdating();
/**
 * @brief Updates RTC information.
 *
 * This function updates the RTC information by reading the values of the RTC registers.
 * @return Non-zero if an error occured.
 */
int rtcUpdate();
/**
 * @brief Converts a BCD (Binary-Coded Decimal) value to binary.
 *
 * This function converts a BCD value to its equivalent binary representation.
 * @param bcd BCD value to be converted.
 * @return Equivalent binary value.
 */
uint8_t _bcdToBinary(uint8_t bcd);

#endif // _PROJ_RTC_H
