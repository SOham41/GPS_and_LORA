//include the libraries

//for GPS
#include<SoftwareSerial.h>
#include<TinyGPS++.h>

//for LoRa
#include<SPI.h>
#include<RH_RF95.h>

//declare constants
static const int RXPin = 4, TXPin = 3, RFM95_CS = 10, RFM95_RST = 9, RFM95_INT = 2, RF95_FREQ = 915.0;
static const uint32_t GPSBaud = 9600;

//Object declarations

//for GPS
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

//for LoRa
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(100);
  
  //for GPS
  ss.begin(GPSBaud);

  //for LoRa
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  delay(100);
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
 
  // Defaults after init are 915.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  //for changing power output
  rf95.setTxPower(23, false);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(ss.available() > 0){
    if(gps.encode(ss.read()))
      sendInfo();
  }
  if(millis() > 5000 && gps.charsProcessed() < 10){
    Serial.println(F("No GPS detected : Check Wiring!"));
    while(true);
  }
}

//sendInfo() function sends the information
void sendInfo(){
  //lat = 12.985784 long = 80.235816 time = 06:43:42 date = 04/11/2017
  //012345678901234567890123456789012345678901234567890123456789012345(+1)
  uint8_t data[] = "lat = ";
  rf95.send(data, sizeof(data));
  rf95.waitPacketSent();
  
}
