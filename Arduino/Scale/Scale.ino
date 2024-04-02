/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updated by chegewara

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 4fafc201-1fb5-459e-8fcc-c5c9c331914b
   And has a characteristic of: beb5483e-36e1-4688-b7f5-ea07361b26a8

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   A connect hander associated with the server starts a background task that performs notification
   every couple of seconds.
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "HX711.h"
#include <SPI.h>
#include <Wire.h>

// OLED Display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//HX711
const int LOADCELL_DOUT_PIN = 9;
const int LOADCELL_SCK_PIN  = 8;
float zero = 0;
float cal = 22.4*0.829;//44.8525;11.213125;
float curread = 0;
float oldread = 0;
float old2read = 0;
float curvalue = 0;
float oldval = 0;
HX711 scale;

// Bluetooth
BLEServer* scaleServer = NULL;
BLECharacteristic* weightCharacteristic = NULL;
BLECharacteristic* rCharacteristic = NULL;
bool deviceConnected = false;
bool newDiscon = true;
// Packet
uint8_t value[10] = {0x03, 0xCA, 0, 0, 0, 0, 0, 0, 0, 0};
int16_t *weight = (int16_t*)&value[2];
int8_t *minutes = (int8_t*)&value[4];
int8_t *seconds = (int8_t*)&value[5];
int8_t *milsecs = (int8_t*)&value[6];

// Variables
//int16_t zero = 0;
int16_t tareValue = 0;
int16_t curW = 0;
int16_t oldW = 0;
uint32_t timer = 0;
uint32_t curT = 0;
uint8_t  timeControl = 0;

#define SCALE_SERVICE_UUID            "0000FFF0-0000-1000-8000-00805F9B34FB" //"9B38F367-5546-A4E1-4DA5-0661A0F71267"
#define WEIGHT_CHARACTERISTIC_UUID    "0000FFF4-0000-1000-8000-00805F9B34FB"
#define RECEIVE_CHARACTERISTIC_UUID   "000036F5-0000-1000-8000-00805F9B34FB"


class scaleServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* scaleServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* scaleServer) {
      deviceConnected = false;
      delay(500); // give the bluetooth stack the chance to get things ready
      scaleServer->startAdvertising(); // restart advertising
//      Serial.println("start advertising");
      timer = millis();
    }
};

// Used to receive commands from the profiling app.
class ReceiveCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *rCharacteristic) {
      std::string packet = rCharacteristic->getValue();
      switch ((uint8_t)packet[1]) {
        case 0x0A:
          if ((uint8_t)packet[2]) {
            //LED On Maybe turn on the screen?
          } else
            // LED Off Maybe turn off the screen?
            break;
        case 0x0B:
          switch ((uint8_t)packet[2]) {
            case 0x00:
              //Timer stop
              timeControl = 32;
              break;
            case 0x03:
              //Timer Start
              timeControl = 0;
              timer = millis();
              break;
          }
          break;
        case 0x0F:
          zero = scale.read_average(10);
          break;
        default:
          break;
      }
      //      Serial.print((uint8_t)packet[0]);
      //      Serial.print((uint8_t)packet[1]);
      //      Serial.print((uint8_t)packet[2]);
      //      Serial.print((uint8_t)packet[3]);
      //      Serial.print((uint8_t)packet[4]);
      //      Serial.print((uint8_t)packet[5]);
      //      Serial.print((uint8_t)packet[6]);
      //      Serial.print((uint8_t)packet.length());
      //      Serial.println();
    }
};

void BLEinit() {
  // create the BLE device
  BLEDevice::init("Decent Scale");

  // Create the server
  scaleServer = BLEDevice::createServer();
  scaleServer->setCallbacks(new scaleServerCallbacks());

  // Create the BLE Service
  BLEService *scaleService = scaleServer->createService(SCALE_SERVICE_UUID);

  // Instanciate characteristics and properties
  weightCharacteristic = scaleService->createCharacteristic(
                           WEIGHT_CHARACTERISTIC_UUID,
                           BLECharacteristic::PROPERTY_READ |
                           BLECharacteristic::PROPERTY_NOTIFY);
  rCharacteristic = scaleService->createCharacteristic(
                      RECEIVE_CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ  |
                      BLECharacteristic::PROPERTY_WRITE);
  // Add callbacks for packets received
  rCharacteristic->setCallbacks(new ReceiveCallbacks());
  weightCharacteristic->addDescriptor(new BLE2902());
  // Start the service
  scaleService->start();
}

void advertiseBLE() {

  // Start advertising
  BLEAdvertising *scaleAdvertising = BLEDevice::getAdvertising();
  scaleAdvertising->addServiceUUID(SCALE_SERVICE_UUID);
  scaleAdvertising->setScanResponse(false);
  scaleAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
}

void initScale() {
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.power_up();
  scale.set_gain(64);
  display.print("Calibrating");
  delay(1000);
  zero = scale.read_average(10);
  // Zero Loop
  while(abs(scale.read()-zero) < 10){
    zero = scale.read_average(3);
    delay(100);
  }
}

void initOLED() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
     
  }
  // init OLED screen
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 0);            // Start at top-left corner
  display.setRotation(2);
}

void setup() { 
  BLEinit();
  initOLED();
  
  timer = millis();
  
  initScale();
  advertiseBLE();
  
  curread = scale.read();
  curW = (uint16_t)((curread - zero) / cal);
  delay(500); // give the bluetooth stack the chance to get things ready
  scaleServer->startAdvertising(); // restart advertising
}

void scaleLoop(){
  display.clearDisplay();
  oldW = curW;
  if (scale.is_ready()) {
    old2read = oldread;
    oldread = curread;
    curread = scale.read();
    oldval = curvalue;
    
    // Filter direct reading
    if (abs(oldread - curread) >= 15 && old2read - curread < 200) {
      curvalue = (2 * curread + 3 * old2read + 3 * oldread) / 8;
      curread = (curread + oldread + old2read) / 3;
    } else {
      curvalue = (5 * curread + old2read + 2 * oldread) / 8;
    }

    // Filter tared and calibrated value
    curW = (uint16_t)((curvalue - zero) / cal);
    if (abs(oldW - curW) < 2) {
      curW = (5 * oldW + 3 * curW) >> 3;
    } else {
      curW = (3 * oldW + 5 * curW) >> 3;
    }

    // Display
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(curW/10.0);
    display.print("g");
    display.display();
  }
  else {
    curW = curW;
  }
}

void loop() {
  scaleLoop();
  // notify changed value
  if (deviceConnected) {
    if (curW != oldW)
      value[1] = 0xCA; // Changing
    else
      value[1] = 0xCE; // Stable

    curT = (millis() - timer) >> (uint32_t)timeControl; // Find current running time or

    // Set timestamp in packet
    *minutes = (uint8_t)(curT / 60000);
    *seconds = (uint8_t)((curT % 60000) / 1000);
    *milsecs = (uint8_t)((curT % 1000) / 100);

    // Set weight in packet to tared weight
    *weight = curW; // - tareValue;
    *weight = (*weight << 8) | (*weight >> 8); // Convert Little-Endian to Big-Endian, RISC-V is little-endian

    // XOR packet for checksum
    value[9] = value[0] ^ value[1] ^ value[2] ^ value[3] ^ value[4] ^ value[5] ^ value[6] ^ value[7] ^ value[8];

    // Send packet
    weightCharacteristic->setValue((uint8_t*)value, 10);
    weightCharacteristic->notify();
    delay(95); // 100ms delay for 10Hz packets, putting 95 for now
  } else {
    if (millis() - timer >= 300000) {
      // Sleep when timed out
      scale.power_down();
      display.clearDisplay();
      esp_deep_sleep_enable_gpio_wakeup(BIT(D2), ESP_GPIO_WAKEUP_GPIO_HIGH);
      esp_deep_sleep_start();
    }
  }
}
