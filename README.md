# STMCode

### MBED OS

We have used mbed os in order to manage the lora transreceiver and the read of analog sensors in a more proficient way.
(Anyway for educational purposes we also make it work with low level HAL libraries.)

### MBED OS Repositories

Mbed os provides its own version control service, we will point to it for the trasmitter and receiver code.
 - [Transmitter](https://os.mbed.com/users/gabrio/code/TRANSMITTER/): Reads analog values and sends it through LoRa 
 - Receiver: Receives LoRa messages and forwards them to a gateway
