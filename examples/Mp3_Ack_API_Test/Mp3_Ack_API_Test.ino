

#define MP3_RX_PIN 13
#define MP3_TX_PIN 15

#if (defined(ARDUINO_AVR_UNO) || defined(ESP8266))   // Using a soft serial port
#include <SoftwareSerial.h>
SoftwareSerial mp3Serial(MP3_RX_PIN, MP3_TX_PIN);
#else
#define mp3Serial Serial1
#endif

// for PlatformIO and ESP8266 best use:
// #include <SoftwareSerial.h>
// EspSoftwareSerial::UART mp3Serial;

#define DebugOut Serial
#define DfMiniMp3Debug DebugOut

#include <DFMiniMp3E.h>

// forward declare the notify class, just the name
//
class Mp3Notify; 

// define a handy type using serial and our notify class
//
typedef DFMiniMp3<Mp3Notify> DfMp3;
// typedef DFMiniMp3<Mp3Notify, Mp3ChipOriginal> DfMp3; // like previous
// typedef DFMiniMp3<Mp3Notify, Mp3ChipMH2024K16SS> DfMp3; // for Chip MH2024K16SS
// typedef DFMiniMp3<Mp3Notify, Mp3ChipMH2024K16SS, 1600> DfMp3; // Chip MH2024K16SS, ACK timeout to 1600 ms
// typedef DFMiniMp3<Mp3Notify, Mp3ChipIncongruousNoAck, 1600> DfMp3; // chip that doesn't support ACK feature
// typedef DFMiniMp3<> DfMp3; // Not use a notify class, chip original, default timeout 900ms

class Mp3Notify
{
public:
  
  // required type
  typedef void TargetType;
  
  // required method even though it doesn't do anything
  static void SetTarget(TargetType*) { }
  
  static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action)
  {
    if (source & DfMp3_PlaySources_Sd) 
    {
      DebugOut.print("SD Card, ");
    }
    if (source & DfMp3_PlaySources_Usb) 
    {
      DebugOut.print("USB Disk, ");
    }
    if (source & DfMp3_PlaySources_Flash) 
    {
      DebugOut.print("Flash, ");
    }
    DebugOut.println(action);
  }
  static void OnError(uint16_t errorCode)
  {
    // see DfMp3_Error for code meaning
    DebugOut.println();
    DebugOut.print("Com Error ");
    DebugOut.println(errorCode);
  }
  
  static void OnPlayFinished([[maybe_unused]] DfMp3_PlaySources source, uint16_t track)
  {
    DebugOut.print("Play finished for #");
    DebugOut.println(track);  
  }
  static void OnPlaySourceOnline(DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "online");
  }
  static void OnPlaySourceInserted(DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "inserted");
  }
  static void OnPlaySourceRemoved(DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "removed");
  }
};


DfMp3 mp3;

uint32_t setupStart;

void setup()
{
  DebugOut.begin(115200);
  
  // serial port initialization
  #if defined(ESP32)
  mp3Serial.begin(9600, SERIAL_8N1, MP3_RX_PIN, MP3_TX_PIN);
  #else
  mp3Serial.begin(9600);
  // or if use EspSoftwareSerial and PlatformIO:
  // mp3Serial.begin(9600, SWSERIAL_8N1, MP3_RX_PIN, MP3_TX_PIN, false);
  #endif
  
  if (!mp3.begin(mp3Serial, /*doReset = */true, /*timeout = */2000)) {  //Use serial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  
  // use hardware serial Serial for MP3 playback on ESP8266
  // move serial to GPIO15(TX) and GPIO13 (RX) for
  // the mp3 module, we loose the debug monitor though
  //Serial.swap();
  
  // mp3.awake();
  // mp3.reset(); 
  
  
  DebugOut.println("initializing...");
  
  
  setupStart = millis();
}

const uint16_t c_playDelay = 1000;

void loop() 
{
  mp3.loop();
  DebugOut.println("starting...");
  
  
  uint16_t version = mp3.getSoftwareVersion();
  DebugOut.print("version ");
  DebugOut.println(version, HEX);
  
  DebugOut.println("playGlobalTrack 6");
  mp3.playGlobalTrack(6);
  delay(c_playDelay);
  
  DebugOut.println("playMp3FolderTrack 2");
  mp3.playMp3FolderTrack(2);
  delay(c_playDelay);
  
  DebugOut.println("playFolderTrack 20, 2");
  mp3.playFolderTrack(20, 2);
  delay(c_playDelay);
  
  DebugOut.println("playFolderTrack16 10, 2000");
  mp3.playFolderTrack16(10, 2000);
  delay(c_playDelay);
  
  DebugOut.println("playRandomTrackFromAll");
  mp3.playRandomTrackFromAll();
  delay(c_playDelay);
  
  DebugOut.println("nextTrack");
  mp3.nextTrack();
  delay(c_playDelay);
  
  DebugOut.println("prevTrack");
  mp3.prevTrack();
  delay(c_playDelay);
  
  uint16_t track = mp3.getCurrentTrack();
  DebugOut.print("current track ");
  DebugOut.println(track);
  
  DebugOut.println("setVolume 16");
  mp3.setVolume(16);
  
  uint16_t volume = mp3.getVolume();
  DebugOut.print("volume ");
  DebugOut.println(volume);
  
  DebugOut.println("increaseVolume");
  mp3.increaseVolume();
  
  DebugOut.println("decreaseVolume");
  mp3.decreaseVolume();
  
  DebugOut.println("loopGlobalTrack 6");
  mp3.loopGlobalTrack(6);
  delay(c_playDelay);
  
  DebugOut.println("loopFolder 20");
  mp3.loopFolder(20);
  delay(c_playDelay);
  
  DebugOut.println("setPlaybackMode 2 (single repeat)");
  mp3.setPlaybackMode(DfMp3_PlaybackMode_SingleRepeat);
  
  DfMp3_PlaybackMode mode = mp3.getPlaybackMode();
  DebugOut.print("playback mode ");
  DebugOut.println(mode);
  
  DebugOut.println("setRepeatPlayAllInRoot false");
  mp3.setRepeatPlayAllInRoot(false);
  delay(c_playDelay);
  
  DebugOut.println("setRepeatPlayCurrentTrack true");
  mp3.setRepeatPlayCurrentTrack(true);
  delay(c_playDelay);
  
  DebugOut.println("setEq 3 (jazz)");
  mp3.setEq(DfMp3_Eq_Jazz);
  
  DfMp3_Eq eqMode = mp3.getEq();
  DebugOut.print("eq mode ");
  DebugOut.println(eqMode);
  
  DebugOut.println("setPlaybackSource 2 (SD)");
  mp3.setPlaybackSource(DfMp3_PlaySource_Sd);
  delay(c_playDelay);
  
  DebugOut.println("start");
  mp3.start();
  delay(c_playDelay);
  
  DebugOut.println("pause");
  mp3.pause();
  delay(c_playDelay);
  
  DebugOut.println("stop");
  mp3.stop();
  delay(c_playDelay);
  
  DfMp3_Status status = mp3.getStatus();
  DebugOut.print("status source is ");
  DebugOut.print(status.source);
  DebugOut.print(" and state is ");
  DebugOut.println(status.state);
  
  uint16_t count = mp3.getFolderTrackCount(10);
  DebugOut.print("folder 10 files ");
  DebugOut.println(count);
  
  count = mp3.getTotalTrackCount();
  DebugOut.print("total files ");
  DebugOut.println(count);
  
  count = mp3.getTotalFolderCount();
  DebugOut.print("total folders ");
  DebugOut.println(count);
  
  DebugOut.println("playAdvertisement 2");
  mp3.playAdvertisement(2);
  delay(c_playDelay);
  
  DebugOut.println("stopAdvertisement");
  mp3.stopAdvertisement();
  delay(c_playDelay);
  
  DebugOut.println("enableDac");
  mp3.enableDac();
  
  DebugOut.println("disableDac");
  mp3.disableDac();
  
  mp3.loop();
  DebugOut.println("...stopping");
  DebugOut.println();
  mp3.loop();
  delay(2000);
}
