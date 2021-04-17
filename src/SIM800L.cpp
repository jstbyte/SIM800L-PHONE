#include "SIM800l.h"

// DEVICE_STATES check_device_state_result(String response)
// {
//     return (DEVICE_STATES)response.charAt(7);
// }

void parse_sim800l_intrupt(SoftwareSerial *serial, eHandler onCall, eHandler onSms)
{
    char respBuffer[64] = {0};
    uint8_t buffIndex = 0;

    while (serial->available())
    {
        respBuffer[buffIndex] = serial->read();
        buffIndex++;
    }

    // After Reading From UART Buffer; //

    String resp = String(respBuffer);

    {
        int indx = resp.indexOf("+CLIP");
        if (indx > 0)
        { // If Incoming Call Then :
            String phone_number = resp.substring(indx + 8, indx + 21);
            if (onCall != NULL)
            {
                (*onCall)(phone_number);
            }
        }
        else
        { // If Incoming Massege Then :
            indx = resp.indexOf("+CMTI");
            if (indx > 0)
            { // Code To Retrive SMS;
                String slot = resp.substring(indx + 12);
                if (onSms != NULL)
                {
                    (*onSms)(slot);
                }
            }
        }
    }
}
