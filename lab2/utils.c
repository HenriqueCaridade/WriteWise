#include <lcom/lcf.h>
#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == 0) { return 1; }
  *lsb = (uint8_t) (val & 0xff);
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == 0) { return 1; }
  *msb = (uint8_t) (val >> 8);
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  if(value == 0) { return 1; }
  uint32_t val;
  if(sys_inb(port, &val)) { return 1; }
  *value = (uint8_t) val;
  return 0;
}
