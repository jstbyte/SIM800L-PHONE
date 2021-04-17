#include <Arduino.h>
#include <SoftwareSerial.h>
#include "SIM800l.h"
#define SIM800L_INT 0

SoftwareSerial sSerial(9, 8);

volatile bool isint = false; // SIM800L Intrupt Flags;
auto callsms_isr = []() { isint = true; };

bool auto_recieve = false;

void setup()
{
  Serial.begin(9600);
  sSerial.begin(9600);
  sSerial.println(SMS_TEXT_MODE); // Text Mode SMS;

  pinMode(2, INPUT_PULLUP);
  attachInterrupt(0, callsms_isr, FALLING);
}

void loop()
{
  // Check for Intrupt and Read;
  if (isint)
  {
    delay(500);
    parse_sim800l_intrupt(
        &sSerial, [](String phone) {
          // Handle Call;
          if(phone == "+919101030411") {
            if(auto_recieve) {
              sSerial.println("ATA");
            }
          } else {
            sSerial.println("ATH");
          }
          Serial.println(phone); },
        [](String slot) {
          // Handle SMS;
          sSerial.println("AT+CMGR=" + slot + ",0");
          delay(100);

          String resp = sSerial.readString();

          if (resp.indexOf("[INFO]") > 0)
          {

            sSerial.println("AT+CBC");
            delay(100);
            resp = sSerial.readString();
            sSerial.println("AT+CSQ");
            delay(100);
            resp += sSerial.readString();
            sSerial.println("AT+CMGS=\"+919101030411\"");
            delay(100);
            sSerial.readString();
            sSerial.println(resp);
            sSerial.write(26);
          }
          else if (resp.indexOf("[AR1]") > 0)
          {
            auto_recieve = true;
          }
          else if (resp.indexOf("[AR0]") > 0)
          {
            auto_recieve = false;
          }

          sSerial.println("AT+CMGDA=\"DEL READ\"");
        });

    isint = false;
  }

  if (Serial.available())
  {
    sSerial.write(Serial.read());
  }
  if (sSerial.available())
  {
    Serial.write(sSerial.read());
  }
}