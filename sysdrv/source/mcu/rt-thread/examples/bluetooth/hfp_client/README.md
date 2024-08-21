RT-Thread HFP Client + A2dp sink demo
======================

Demo of HFP Client + A2dp sink demo

This is the demo of API implementing Advanced Audio Distribution Profile to receive an audio stream.

This example involves the use of Bluetooth legacy profile A2DP for audio stream reception, AVRCP for media information notifications, and I2S for audio stream output interface.

Applications such as bluetooth speakers can take advantage of this example as a reference of basic functionalities.

## How to use this example

### Hardware Required

To play the sound, there is a need of loudspeaker and possibly an external I2S codec. Otherwise the example will only show a count of audio data packets received silently. Internal DAC can be selected and in this case external I2S codec may not be needed.

For the I2S codec, pick whatever chip or board works for you; this code was written using a PCM5102 chip, but other I2S boards and chips will probably work as well. The default I2S connections are shown below, but these can be changed in menuconfig:

If the internal DAC is selected, analog audio will be available on GPIO25 and GPIO26. The output resolution on these pins will always be limited to 8 bit because of the internal structure of the DACs.

### Configure the project

```
scons --menuconfig
```

Configure the protocol stack and open the hfp and a2dp function.

```
RT-Thread Components  --->
	Bluetooth  --->
        [*] 	Bluedroid Enable  --->
            [*]   Classic Bluetooth (NEW)
            [*]     A2DP
            (2048)    A2DP sink (audio stream decoding) task stack size (NEW)
            (2048)    A2DP source (audio stream encoding) task stack size (NEW)
            [*]     Hands Free/Handset Profile
                Hands-free Profile Role configuration (Hands Free Unit)  --->
                audio(SCO) data path (PCM)  --->
```

Open a2dp source example.

```
RT-Thread Components  --->
    Bluetooth  --->
        examples bluetooth  --->
            Example 'HFP CLIENT' Config  --->
                [*] Demo of hfp client
```


## Example Output

After the program is started, the example starts inquiry scan and page scan, awaiting being discovered and connected. Other bluetooth devices such as smart phones can discover a device named "RK_SPEAKER". A smartphone or another RT-Thread example of A2DP source can be used to connect to the local device.

Once A2DP connection is set up, there will be a notification message with the remote device's bluetooth MAC address like the following:

```
I (106427) BT_AV: A2DP connection state: Connected, [64:a2:f9:69:57:a4]
```

If a smartphone is used to connect to local device, starting to play music with an APP will result in the transmission of audio stream. The transmitting of audio stream will be visible in the application log including a count of audio data packets, like this:

```
I (120627) BT_AV: A2DP audio state: Started
I (122697) BT_AV: Audio packet count 100
I (124697) BT_AV: Audio packet count 200
I (126697) BT_AV: Audio packet count 300
I (128697) BT_AV: Audio packet count 400

```

Also, the sound will be heard if a loudspeaker is connected and possible external I2S codec is correctly configured. For A2DP source example, the sound is noise as the audio source generates the samples with a random sequence.

## Troubleshooting
* For current stage, the supported audio codec in A2DP is SBC. SBC data stream is transmitted to A2DP sink and then decoded into PCM samples as output. The PCM data format is normally of 44.1kHz sampling rate, two-channel 16-bit sample stream. Other decoder configurations in A2DP sink is supported but need additional modifications of protocol stack settings.
* As a usage limitation, A2DP sink can support at most one connection with remote A2DP source devices. Also, A2DP sink cannot be used together with A2DP source at the same time, but can be used with other profiles such as SPP and HFP.