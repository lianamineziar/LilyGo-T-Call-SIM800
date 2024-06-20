// Please select the corresponding model

 // Use the ESP8266WiFi library for NodeMCU

// #define SIM800L_IP5306_VERSION_20190610
//#define SIM800L_AXP192_VERSION_20200327
 #define SIM800C_AXP192_VERSION_20200609

// Define the serial console for debug prints, if needed
#define DUMP_AT_COMMANDS
#define TINY_GSM_DEBUG          SerialMon

#include "utilities.h"

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to the module)
#define SerialAT  Serial1

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800          // Modem is SIM800
#define TINY_GSM_RX_BUFFER      1024   // Set RX buffer to 1Kb

#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif


// Set phone numbers, if you want to test SMS and Calls
#define SMS_TARGET  "+918082923930"
#define CALL_TARGET "+380xxxxxxxxx"
 
const int sensorPin = 36;

void setup()
{
    // Set console baud rate
    SerialMon.begin(115200);

    delay(10);

    // Start power management
    if (setupPMU() == false) {
        Serial.println("Setting power error");
    }

    // Some start operations
    setupModem();

    // Set GSM module baud rate and UART pins
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);

    delay(6000);
}

void loop()
{
    // Read sensor data
    
    int sensorValue = analogRead(sensorPin);
    float temperatureC = sensorValue * 0.48875;  // Convert analog reading to temperature in Celsius

    // Format the data to send
    String message = "Temperature: " + String(temperatureC, 2) + " °C";

    // Send data via SMS
    SerialMon.println("Sending SMS...");
    bool res = modem.sendSMS(SMS_TARGET, message);

    if (res) {
        SerialMon.println("SMS sent successfully!");
    } else {
        SerialMon.println("Failed to send SMS.");
    }

    delay(60000); // Delay for 1 minute (60000 ms) before sending again
}
