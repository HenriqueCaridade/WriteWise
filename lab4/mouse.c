#include <lcom/lcf.h>
#include <lcom/utils.h>
#include "i8042.h"
#include "KBC.h"

int hookIdMouse = 3;
struct packet mousePacket;
uint8_t byteIndex = 0;
uint8_t mouseBytes[3];
uint8_t currentByte;

int (mouse_subscribe_int)(uint8_t *bit_no){
  if (bit_no == 0) return 1;
  *bit_no = BIT(hookIdMouse);
  return sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookIdMouse);
}

int (mouse_unsubscribe_int)() {
  return sys_irqrmpolicy(&hookIdMouse);
}

void (mouse_ih)(){
  if (read_KBC_output(KBC_OUT_CMD, &currentByte, true)) printf("Error in reading byte from mouse.");
}

void (mouse_sync_bytes)(){
  if (byteIndex == 0 && (currentByte & FIRST_BYTE)) mouseBytes[byteIndex++] = currentByte;
  else if (byteIndex > 0) mouseBytes[byteIndex++] = currentByte;
}

void (mouse_bytes_to_packet)(){
  for (int i = 0; i < 3; i++) mousePacket.bytes[i] = mouseBytes[i]; 

  mousePacket.lb = mouseBytes[0] & MOUSE_LB;
  mousePacket.mb = mouseBytes[0] & MOUSE_MB;
  mousePacket.rb = mouseBytes[0] & MOUSE_RB;
  mousePacket.x_ov = mouseBytes[0] & MOUSE_X_OVERFLOW;
  mousePacket.y_ov = mouseBytes[0] & MOUSE_Y_OVERFLOW;
  mousePacket.delta_x = (mouseBytes[0] & MOUSE_X_SIGNAL) ? (0xFF00 | mouseBytes[1]) : mouseBytes[1];
  mousePacket.delta_y = (mouseBytes[0] & MOUSE_Y_SIGNAL) ? (0xFF00 | mouseBytes[2]) : mouseBytes[2];
}

int (mouse_write)(uint8_t command){
  uint8_t mouse_response = 0;
  uint8_t attempts = MAX_ATTEMPTS;

  while (attempts-- && mouse_response != ACK) {
    if (write_KBC_command(KBC_IN_CMD, WRITE_BYTE_MOUSE)) return 1;
    if (write_KBC_command(KBC_OUT_CMD, command)) return 1;
    tickdelay(micros_to_ticks(WAIT_KBC));
    if (util_sys_inb(KBC_OUT_CMD, &mouse_response)) return 1;
    if (mouse_response == ACK) return 0;
  }
  return 1;
}
