// this example will play a random track from all on the sd 
//
// it expects the sd card to contain some mp3 files

// serial dfPlayer definition 
#define MP3_RX_PIN 13
#define MP3_TX_PIN 15

#if (defined(ARDUINO_AVR_UNO) || defined(ESP8266))   // Using a soft serial port
  #include <SoftwareSerial.h>
  SoftwareSerial mp3Serial(MP3_RX_PIN, MP3_TX_PIN);
#else
  #define mp3Serial Serial1
#endif


#include <DFMiniMp3E.h>

// define a handy type using hardware serial with no notifications
//
typedef DFMiniMp3<> DfMp3; 

// instance a DfMp3 object, 
//
DfMp3 dfmp3;

void setup() 
{
  Serial.begin(115200);

  Serial.println("initializing...");

// serial port initialization
#if (defined ESP32)
  mp3Serial.begin(9600, SERIAL_8N1, MP3_RX_PIN, MP3_TX_PIN);
#else
  mp3Serial.begin(9600);
#endif

  if (!dfmp3.begin(mp3Serial, /*doReset = */true, /*timeout = */2000)) {  //Use serial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }

  // during development, it's a good practice to put the module
  // into a known state by calling reset().  
  // You may hear popping when starting and you can remove this 
  // call to reset() once your project is finalized
  // dfmp3.reset(); 
  
  uint16_t version = dfmp3.getSoftwareVersion();
  Serial.print("version ");
  Serial.println(version);

  // show some properties and set the volume
  uint16_t volume = dfmp3.getVolume();
  Serial.print("volume ");
  Serial.println(volume);
  dfmp3.setVolume(24);
  
  uint16_t count = dfmp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
  Serial.print("files ");
  Serial.println(count);

  uint16_t mode = dfmp3.getPlaybackMode();
  Serial.print("playback mode ");
  Serial.println(mode);
  
  Serial.println("starting...");
  
  dfmp3.playRandomTrackFromAll(); // random of all folders on sd
}

void loop() 
{
  // calling dfmp3.loop() periodically allows for notifications 
  // to be handled without interrupts
  dfmp3.loop();
}
