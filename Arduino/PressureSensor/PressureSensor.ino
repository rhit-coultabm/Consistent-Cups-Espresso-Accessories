#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
BLECharacteristic* zCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
int16_t value = 0;
int16_t zero = 0;
uint32_t disconnected = 0;

// Characteristics defined by PRS (https://pressensor.com/pages/prs-protocol)
#define PRESSURE_SERVICE_UUID        "873ae82a-4c5a-4342-b539-9d900bf7ebd0"
#define PRESSURE_CHARACTERISTIC_UUID "873ae82b-4c5a-4342-b539-9d900bf7ebd0"
#define ZERO_CHARACTERISTIC_UUID     "873ae82c-4c5a-4342-b539-9d900bf7ebd0"


class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      delay(500); // give the bluetooth stack the chance to get things ready
      pServer->startAdvertising(); // restart advertising
      Serial.println("start advertising");
      oldDeviceConnected = deviceConnected;
      zCharacteristic->setValue((uint8_t*)&zero, 2);
      pCharacteristic->setValue((uint8_t*)&zero, 2);
    }
};

// Used to set the zero value in the profiling app.
class ZeroCallbacks: public BLECharacteristicCallbacks {
    void onRead(BLECharacteristic *zCharacteristic) {
      zCharacteristic->setValue((uint8_t*)&zero, 2);
      pCharacteristic->setValue((uint8_t*)&zero, 2);
    }
};


void setup() {
//  Serial.begin(115200); // DEBUG

  // Create the BLE Device, name required for in-app detection
  BLEDevice::init("PRS");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(PRESSURE_SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      PRESSURE_CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY 
                    );
  zCharacteristic = pService->createCharacteristic(
                      ZERO_CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ  |
                      BLECharacteristic::PROPERTY_WRITE 
                    );
  zCharacteristic->setCallbacks(new ZeroCallbacks());

  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // Set initial value to zero so that the app can be calibrated properly. 
  zCharacteristic->setValue((uint8_t*)&zero, 2);
  pCharacteristic->setValue((uint8_t*)&zero, 2);
  analogReadResolution(12); // 12 Bit read resolution to maximize precision when reading the pressure sensor.
  disconnected = millis();
  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(PRESSURE_SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
//  Serial.println("Waiting a client connection to notify..."); // DEBUG
}

void loop() {
    // notify changed value
    if(deviceConnected){
      while (deviceConnected) {
        value = analogReadMilliVolts(A2);
        // Convert ADC output to millibar, 0 PSI default to 340, divide by 0.2 to scale to millibar
        value = (int16_t)((float)(value-340)/0.2);
        value = (value<<8)|(value>>8); // Convert Little-Endian to Big-Endian, RISC-V is little-endian
        pCharacteristic->setValue((uint8_t*)&value, 2);
        pCharacteristic->notify();
        delay(20);
      }
      disconnected = millis();
    } else if(millis()-disconnected >= 60000){
        // Wake on active-high signal on pin 2
        esp_deep_sleep_enable_gpio_wakeup(BIT(D2), ESP_GPIO_WAKEUP_GPIO_HIGH);
        esp_deep_sleep_start();
    }
}
