# What is High Voltage Programming ?
High Voltage Programming (HVP) is a method of programming microcontrollers and other electronic devices by applying a high voltage signal to the device's programming pins. This high voltage signal is typically in the range of 10-15 volts and is used to enable special programming modes in the device.

# Why it is use ?
HVP is used when the normal programming interface of the device is not accessible or when the device is in a locked state, preventing normal programming. HVP is also used to program devices that have been damaged or erased, allowing the device to be reprogrammed.

# How High Voltage Programming done ?
The HVP process involves applying a high voltage signal to the device's programming pins while providing the necessary programming data and commands through a programming tool or software. The programming data is loaded into the device's memory and the device is then reset to begin executing the newly programmed code. HVP should be performed with caution as it involves applying high voltage signals to electronic devices, which can potentially damage the device if not done correctly. It is recommended that HVP is only performed by experienced and qualified technicians.

# Arduino-HV_Programmer
This is a Arduino based High Voltage Fuse Resetter. This can reset the fuse of At-Mega 328P/328/8 or At-Tiny 85/13. This needs on a 12v DC Power source.
We can control this device using a software. which is given with its screen shots.

# Hardware Design:
To create my high voltage programming device, I started with an Arduino Uno board. The Arduino board is connected to an external high voltage programming circuit, which includes a DC-DC converter, Two IC sockets are required for the 8 pin(AT-tiny) and 28 pin(AT-Mega328P-PU) microcontrollers, and some resistors and capacitors. The DC-DC converter is used to generate the high voltage signal required for HVP. The IC socket allows for easy insertion and removal of the microcontroller being programmed. The resistors and capacitors are used to filter and regulate the high voltage signal.
</br><center><img src="https://github.com/shuvabratadey/Arduino-HV_Programmer/blob/main/Arduino-HV-Programmer-photos/IMG_1.jpg" width="500" hight="450"/></center>
<center><img src="https://github.com/shuvabratadey/Arduino-HV_Programmer/blob/main/Arduino-HV-Programmer-photos/IMG_2.jpg" width="500"/></center>

# Software Design:
To control the Arduino board and trigger the HVP process, I developed a software application using Visual Studio. The program was written in C# and used the Serial library to communicate with the Arduino board via a serial connection. The program sends the appropriate commands and data to the Arduino board to trigger the HVP process. This includes setting the programming mode, sending the high voltage signals, and programming the microcontroller.
![Software Image](https://github.com/shuvabratadey/Arduino-HV_Programmer/blob/main/image.png)

# Safety Considerations:
It's important to note that implementing HVP requires additional hardware and careful attention to safety, as it involves high voltage signals. Therefore, I took the necessary precautions when designing and implementing the HVP circuitry and programming process. This includes using proper insulation, ensuring that the high voltage source is well isolated from other components, and wearing appropriate protective equipment when handling the circuitry.

# Conclusion:
In conclusion, My High Voltage Programming Device is an Arduino-based device that allows for programming of microcontrollers using HVP. The device is controlled by a software application developed using Visual Studio, which sends the appropriate commands and data to the Arduino board to trigger the HVP process. While implementing HVP requires additional hardware and safety considerations, it can be a useful method for programming microcontrollers and other electronic devices that require HVP.
