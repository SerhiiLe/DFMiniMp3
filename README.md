# DFMiniMp3

Arduino library for the DFPlayer Mini Mp3 module.

This is a copy of the project [Makuna/DFMiniMp3](https://github.com/Makuna/DFMiniMp3/wiki) with minor changes for better adaptation to my project. The original author is Michael Miller.

## API Reference

There are some minor changes compared to the original.

## Construction and Template Arguments

The constructor will require the declaration of several template arguments that specialize the class to the sketch writer's needs.  Some of these class types are provided and some are required by the user to provide.

### DFMiniMp3\<T_NOTIFICATION_METHOD, T_CHIP_VARIANT, C_ACK_TIMEOUT\>

_T_NOTIFICATION_METHOD_ - the class type of the notification handler, you must provide this.  Please read [Implementing a notification method](//github.com/Makuna/DFMiniMp3/wiki/Notification-Method) for more details.  The examples also demonstrate simple implementations.  
_T_CHIP_VARIANT_ - (optional) the class type of the chip variant you have, usually either `Mp3ChipOriginal` or `Mp3ChipMH2024K16SS`.  The default is Mp3ChipOriginal.  In the rare case you have a chip that doesn't support ACK feature, then you use `Mp3ChipIncongruousNoAck`.  
_C_ACK_TIMEOUT_ - (optional) the value in milliseconds to use for serial timeout when waiting for an ACK from the chip.  The default is 900 ms.  

The following example uses the user provided callback class named `Mp3Callbacks`.  

```cpp
DFMiniMp3<HardwareSerial, Mp3Callbacks> mp3;
```

The following example uses the user provided callback class named `Mp3Callbacks` specialized for the MH2024K16SS chip.  

```cpp
DFMiniMp3<Mp3Callbacks, Mp3ChipMH2024K16SS> mp3;
```

The following example sets the ACK timeout to 1600 ms if your chip requires a longer serial timeout.  

```cpp
DFMiniMp3<Mp3Callbacks, Mp3ChipMH2024K16SS, 1600> mp3;
```

## Methods  

### bool begin(Stream &serial, bool doReset = true, unsigned int timeout = 2000)  

This will initialize the DFMiniMp3 and prepare it for use.  Call this within the sketches `setup()`.  

_serial_ - the instance of the serial  
_doReset_ - reset the module during initialization (recommend)  
_timeout_ - timeout of waiting for response from module in msec  

Return _true_ if module is online.

Initialization the hardware serial instance:

```cpp
#include <HardwareSerial.h>
HardwareSerial mp3Serial(1);
```

Initialization the software serial instance:

```cpp
#include <SoftwareSerial.h>
SoftwareSerial mp3Serial(rxPin, txPin);
```

or

```cpp
#include <SoftwareSerial.h>
EspSoftwareSerial::UART mp3Serial;
```

if you use the EspSoftwareSerial (recommend for ESP8266)

in global section, and

```cpp
mp3Serial.begin(9600, SERIAL_8N1, rxPin, txPin);
```

for hardware serial, or

```cpp
mySerial.begin(9600);
```

for the SoftwareSerial, or

```cpp
mp3Serial.begin(9600, SERIAL_8N1, rxPin, txPin, false);
```

for EspSoftwareSerial

_9600_ - the baud rate the module communicates at by defaults.  It is rare you will need to provide this.  
_rxPin_ - the receive pin for the serial.  
_txPin_ - the transmit pin for the serial.  

### void setComRetries(uint8_t retries)  

This changes the number retries used when the module has communications problems.  The default is 3 and this should not need to be called.  

### void loop()  

This will service the DFMiniMp3, listening for responses from the hardware.  Call this often within the sketches `loop()`.

### DfMp3_PlaySources getPlaySources()  

**DEPRECATED** - the return communication is the same as the play source online notification, and thus will cause issues.  
This will return the active play sources.  This method is not supported with all the generally compatible chips.  It will work with the YXS200-24SS chip.  It will not work with the MH2024K-24SS chip.  
The return value is a bit field value with any of the following possible fields set.  
    DfMp3_PlaySources_Usb -  
    DfMp3_PlaySources_Sd -  
    DfMp3_PlaySources_Pc -  
    DfMp3_PlaySources_Flash -  

### uint16_t getSoftwareVersion()  

This will return current software/firmware version used within the MP3 module.  

### void playGlobalTrack(uint16_t track)  

This will play the given global track.  
_track_ - the global track number.  A global track number is the number of the file as enumerated from all folders on the SD card.  It has no relationship to the file name.  You can use the `getTotalTrackCount()` to obtain the number of tracks enumerated.  

### void playMp3FolderTrack(uint16_t track)  

This will play the track from within the mp3 folder.  
An example filename would be sd:/mp3/0013_ThisIsMyFavoriteSong.mp3, and calling `playMp3FolderTrack(13)` would play it.  
_track_ - the track number as listed in the filename.  The filename must start with a four digit, zero padded number and any characters that follow will be ignored.  


### void playFolderTrack(uint8_t folder, uint8_t track)  

This will play the track from within the specific numbered folder.  
An example filename would be sd:/02/013_ThisIsMyFavoriteSong.mp3, and calling `playFolderTrack(2, 13)` would play it.  
_folder_ - the folder number as listed in the folder name.  The foldername must be a three digit on some chips and two digits on newer chips, zero padded number.  
_track_ - the track number as listed in the track filename.  The filename must start with a three digit, zero padded number and any characters that follow will be ignored.  

### void playFolderTrack16(uint8_t folder, uint16_t track)  

This will play the track from within the specific numbered folder.  
An example filename would be sd:/08/2013_ThisIsMyFavoriteSong.mp3, and calling `playFolderTrack16(8, 2013)` would play it.  
_folder_ - the folder number as listed in the folder name.  The foldername must be a two digit, zero padded number.  
_track_ - the track number as listed in the track filename.  The filename must start with a four digit, zero padded number and any characters that follow will be ignored.  

### void playRandomTrackFromAll()  

This will play a random track from all tracks on the SD card.  This will include all tracks from all folders including the advertisement folder.  

### void nextTrack()  

This will cause the current track to stop and the next track to start playing immediately.  

### void prevTrack()  

This will cause the current track to stop and the previous track to start playing immediately.  

### uint16_t getCurrentTrack(DfMp3_PlaySource source)  

This will return the current playing track from the given source.  
 _source_ - One of the following values:  
    DfMp3_PlaySource_Usb -  
    DfMp3_PlaySource_Sd -  (the default)  
    DfMp3_PlaySource_Aux -  
    DfMp3_PlaySource_Sleep -  
    DfMp3_PlaySource_Flash -  

### void setVolume(uint8_t volume)  

This will set the playback volume.  
_volume_ - (0-30) the volume level to set.  

### uint8_t getVolume()  

This will return the current volume.  

### void increaseVolume()  

This will increase the volume by 1.  

### void decreaseVolume()  

This will decrease the volume by 1.  

### void loopGlobalTrack(uint16_t globalTrack)  

This will start playing the given globalTrack and have it loop continuously.  When the song finishes, it will restart at the beginning.  
_globalTrack_ - the global track number.  A global track number is the number of the file as enumerated from all folders on the SD card.  It has no relationship to the file name.  You can use the `getTotalTrackCount()` to obtain the number of tracks enumerated.  

### void loopFolder(uint8_t folder)  

This will start playing the tracks within the given folder and have it loop continuously playing all songs within the folder.  
_folder_ - (0-99) the folder number in the root represented as two digits zero padded.  

### DfMp3_PlaybackMode getPlaybackMode()  

Retrieve the current playback mode.  It will be one of the following.  
    DfMp3_PlaybackMode_Repeat -  
    DfMp3_PlaybackMode_FolderRepeat -  
    DfMp3_PlaybackMode_SingleRepeat -  
    DfMp3_PlaybackMode_Random -  

### void setRepeatPlayCurrentTrack(bool repeat)  

Set the mode to repeat play any file that is playing.  If set, the mp3 will loop continuously.  If not set, it will play once and then stop.  

### void setRepeatPlayAllInRoot(bool repeat)  

Set the mode to repeat play all files that are the root of the media.  If set, it will loop continuously.  If not set, it will play once and then stop.  

### void setEq(DfMp3_Eq eq)  

Set the equalizer mode.  
 _eq_ One of the equalizer modes.  
    DfMp3_Eq_Normal -  
    DfMp3_Eq_Pop -  
    DfMp3_Eq_Rock -  
    DfMp3_Eq_Jazz -  
    DfMp3_Eq_Classic -  
    DfMp3_Eq_Bass -  

### DfMp3_Eq getEq()  

Get the current equalizer mode.  
returns one of these values.  
    DfMp3_Eq_Normal -  
    DfMp3_Eq_Pop -  
    DfMp3_Eq_Rock -  
    DfMp3_Eq_Jazz -  
    DfMp3_Eq_Classic -  
    DfMp3_Eq_Bass -  

### void setPlaybackSource(DfMp3_PlaySource source)  

Sets the playback source.  Generally avoid calling this unless you know what you are doing.  
 _source_ - One of the following values:  
    DfMp3_PlaySource_Usb -  
    DfMp3_PlaySource_Sd -  the most common value  
    DfMp3_PlaySource_Aux -  
    DfMp3_PlaySource_Sleep -  
    DfMp3_PlaySource_Flash -  

### void sleep()  

Cause the Mp3 module to go into a very low power mode.  This is useful for battery operated devices when long periods of no sound will be needed.  
 **Note:**  
 There are two confirmed ways to wake up a module. Try any of the two and see which works best for your particular hardware:  

* Calling `reset()`, although this is a slow process and may cause click noises in the audio output  
* Calling `setPlaybackSource(DfMp3_PlaySource_Sd)`  

### void reset(bool waitForOnline = true, unsigned int timeout = 2000)  

Reset the hardware module.  Calling this method will make the module not available for about one second and may cause noise in the audio output.  
During software development it is a good practice to call this right after begin() to put the MP3 Module into a known state.  In a final product, there is an assumption that the uC and the MP3 Module will be powered on together and thus will start in a known state; so it can be removed then.  
_waitForOnline_ - wait for module online  
_timeout_ - timeout of waiting for response from module in msec  

### void start()  

This will resume playing the current track where it was paused at.  Some modules have required calling this before anything is played.  

### void pause()  

This will pause the current playing track.  

### void stop()  

This will stop the current playing track.  

### DfMp3_Status getStatus()  

This will return the current status.  

```cpp
struct DfMp3_Status {
    DfMp3_StatusSource source;
    DfMp3_StatusState state;
};
```

_source_ - one of the following  
DfMp3_StatusSource_General - some modules don't provide specific sources so they will return this  
DfMp3_StatusSource_Usb -  
DfMp3_StatusSource_Sd -  
DfMp3_StatusSource_Sleep -  some modules the source will also return this state  

_state_ - one of the following  
DfMp3_StatusState_Idle -  
DfMp3_StatusState_Playing -  
DfMp3_StatusState_Paused -  
DfMp3_StatusState_Sleep -  
DfMp3_StatusState_Shuffling - switching tracks to continue playing  

### uint16_t getFolderTrackCount(uint16_t folder)  

This will return the count of tracks in the given folder  
_folder_ - the folder number to enumerate files within.  
**NOTE:**  If the count returned doesn't match your expectations, [please read this discussion](https://github.com/Makuna/DFMiniMp3/discussions/142) where one chip returns the last accessed folder and ignores the passed in folder id.

### uint16_t getTotalTrackCount(DfMp3_PlaySource source)  

This will return the count of tracks in all folders, including advertisements.  
_source_ - the media source to enumerate.  One of the following values.  
    DfMp3_PlaySource_Usb -  through the USB interface if present  
    DfMp3_PlaySource_Sd -  on the SD card, the most common source  
    DfMp3_PlaySource_Flash -  on the flash memory soldered on the board if present  

### uint16_t getTotalFolderCount()

This will return the count of folders in the root of the media.  

### void playAdvertisement(uint16_t track)  

This will pause the current playing track, then play the given advertisement track, and when the advertisement track is finished, it will resume the original track where it was interrupted.  
An example filename would be sd:/advert/0042_BuyHitchHikersGuideFromAmazon.mp3, and calling `playAdvertisement(42)` would play it.  
_track_ - the track number as listed in the filename.  The filename must start with a four digit, zero padded number and any characters that follow will be ignored.  

### void stopAdvertisement()  

This will stop the current playing advertisement and resume the original interrupted track.  

### void disableDac()  

This will disable the onboard DAC.  
 **NOTE:**  This method does seem to work across most chips used on these modules, but it is not documented for all of them and may not work consistently.  

### void enableDac()  

This will enable the onboard DAC.  
 **NOTE:**  This method does seem to work across some chips used on these modules, but it is not documented for all of them and has been demonstrated to not work consistently.  

### bool isOnline()  

Will return `true` if some media play source is online and ready to play.  
Construction and Template Arguments
