#include <Arduino.h>
#include <SoftwareSerial.h>

#define SMS_TEXT_MODE "AT+CMGF=1"
#define DELETE_ALL_MSG "AT+CMGDA=\"DEL ALL\""

// // Functions //
// enum DEVICE_STATES
// {
//     READY,
//     UNKNOWN,
//     RINGING,
//     IN_CALL
// };

// DEVICE_STATES check_device_state_result(String response);

// Call & SMS Parser And Event Emmiter;
typedef void (*eHandler)(String arg);
void parse_sim800l_intrupt(SoftwareSerial *serial, eHandler onCall, eHandler onSms);