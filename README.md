
# Dagu Thumper Tour Guide
The main idea of this project is to design a conceptual robot tour guide for an art gallery using Dagu Thumper. The kit's task is to scan the art space, detect the exibits by detecting their color, recall background information and give the visitors a brief description about the history of the art piece, e.x. artist, period, location ..etc.
<p align="center">
<img src="https://github.com/HadeelMabrouk/Embedded-Project-2/blob/main/Images/tour.jpg" width="700" height="400">
<p>

## Motivation 
Social robots has been recently used to serve multiple purposes, like personal assistance and performing tasks within the domain of domestic services and healthcare. Currently, there is a growing interest in the use of tour guide mobile robots in various environments such as museums, exhibitions and fairs, to provide help for the visitors by offering them a set of services, such as giving them general tours, helping them find points of interest, and providing information about different exhibits.

## Initial Design 
### Features
* MP3 Audio Playing: To give a brief description about an art piece.
* Color Detection: To recognize the art piece through dtecting different colors.
* Line Following: To use a line tracker array to follow a certain path.
* User Input Taking: By using an HC-05 bluetooth bridge.

### Logic Design 
The Kit will take the user input through a bleutooth module. The Dagu will then start the track searching for the spicific color associated with this exhibit. When the Robot finds the exhibit it will stop and retrieve the information related to it. A black track will represent the exhibition hall and two IR arrays will be used to follow this track. 


<p align="center">
<img src="https://github.com/HadeelMabrouk/Embedded-Project-2/blob/main/Images/logic.PNG" width="700" height="400">
<p>
  
### System Design
The following is a grahical representation of the full system design showing all the sensors and controllers. 
<p align="center">
<img src="https://github.com/HadeelMabrouk/Embedded-Project-2/blob/main/Images/system.png" width="700" height="700">
<p>

### Physical Connections 
The following is a real representation of the whole system on top of the Dago Thumper kit. 
<p align="center">
<img src="https://github.com/HadeelMabrouk/Embedded-Project-2/blob/main/Images/connections.jpg" width="650" height="500">
<p>
<p align="center">
<img src="https://github.com/HadeelMabrouk/Embedded-Project-2/blob/main/Images/connections2.jpg" width="650" height="700">
<p>
IR array sensor fixation to read the black line:
<p align="center">
<img src="https://github.com/HadeelMabrouk/Embedded-Project-2/blob/main/Images/IR_array.jpg" width="650" height="450">
<p>
  


## Hardware Components    
* 2 STM32 Nucleo Boards (L432KC): to controle the communication between all the actuators and sensors. 
* Dagu Thumper
* Pololu TReX Motor Controller : To control the Dagu Thumper.
* Color Sensor (TCS34725): to detect and recognize a certain exhibit.
* MP3 Mini Player Module (MP3－TF－16P): to play an audio file describing a given exhibit.
* MicroSD Memory Card: for audio files data storage.
* Speakers: To retrieve information. 
* Line Follower Sensor: to help the Dagu Thumper follow a given path.
* HC-05 Bluetooth Bridge: to take input from the user about the exhibit they want to visit.
* Android Smart Phone: to send commands to the system about the exhibit they want to visit.

## Sensors and Actuators

### Color Sensors

#### TCS3200 Color Sensor
<p align="center">
<img src="https://ram-e-shop.com/wp-content/uploads/2018/09/sen_tcs3200.jpg" width="320" height="240">
<p>
Initially, we wanted to design our system using the TCS3200 color sensor, which works as follows. It first senses color light with the help of an 8 x 8 array of photodiodes. Then, using a Current-to-Frequency Converter the readings from the photodiodes are converted into a square wave with a frequency directly proportional to the light intensity. Finally, using the Microcontroller Board we can read the square wave output and get the results for the color.

How it Works:
* First set the input pins as input and output pins as output. No need to use analog pins.
* Set S0 and S1 to high or low to set desired frequency scaling.
* In loop, activate each filters by setting S2 and S3 to HIGH or LOW and measure frequency ‘fo’ from 6th pin to get corresponding colour intensity. Compare frequencies of each colour to determine the colour of the object.

Each 16 photodiodes are connected in parallel, so using the two control pins S2 and S3 we can select which of them will be read. So for example, if we want to detect red color, we can just use the 16 red filtered photodiodes by setting the two pins to low logic level according to the following table: 

<p align="center">
<img src="https://github.com/HadeelMabrouk/Embedded-Project-2/blob/main/Images/table1.png" width="500" height="140">
<p>
  
The sensor has two more control pins, S0 and S1 which are used for scaling the output frequency. The frequency can be scaled to three different preset values of 100 %, 20 % or 2%. This frequency-scaling function allows the output of the sensor to be optimized for various frequency counters or microcontrollers.

#### TCS34725 Color Sensor
<p align="center">
<img src="https://superbtech.in/wp-content/uploads/2020/07/1-57.jpg" width="400" height="380">
<p>
After running our initial experiments using the TCS3200 sensor, we later decided to replace it with the TCS34725 color sensor due to the fact that it is provided with an IR blocking filter, integrated on-chip and localized to the color sensing photodiodes, that minimizes the IR spectral component of the incoming light and allows color measurements to be made accurately. Additionally, it can be easily integrated with our Nucleo Board since it doesn't depend on timers to measure color frequencies, unlike TCS3200. To integrate it in our system, we made use of the library provided by Adafruit, and made the necessary adjustments to make it compatible with our C project since the library is implemented in C++. I2C synchronous serial communication was used to interface it with the MCU.
  
### Line Follower Sensor
<p align="center">
<img src="https://i0.wp.com/makerselectronics.com/wp-content/uploads/2017/01/Line-Tracker-Module-3-Channels.jpg?fit=1600%2C1600&ssl=1" width="420" height="430">
<p>
  
  The tracing module sensor has 3 way signal collection and output, respectively the left, the middle, and the right of the 3 pair’s infrared TCRT5000 photoelectric switches.  The communication with STM32 is directly through the GPIO port communication. The MCU handles the signals by collecting the signals from the 3 way sensor digital signal port.  When the signal from the TCRT5000 was reflected by a white object it outputs high level.  When meeting the black line, the infrared signal will be absorbed which won’t be reflected back. Hence, signal port will output low level. We used two modules to accomodate for the high speed of the Dagu sinse using one module only wasn't effecient in reading the line correctly. 

### MP3 Mini Player Module
<p align="center">
<img src="https://i2.wp.com/electra.store/wp-content/uploads/2020/07/dfplayer_mini_mp3_player_module_-_twins_chip_1-550x550w.jpg?fit=550%2C550&ssl=1" width="275" height="275">
<p>
The mp3 module is connected to the microcontroller using an asynchronous serial link, where it can read audio mp3 files from the microSD memory card and display them using the speaker, by taking command from the microcontroller on which audio file to play, volume level, ..etc. We connected it to the STM32 board using asynchronous serial communication, namely UART2, with the default baud rate for the module (9600), as well as the default clock configurations. Additionally, we made use of aready made library , and added to it some functionality to play a certain audio file by taking the track number as a parameter. For more information, check out the tutorial available in the references section below.
  
### HC-05 Bluetooth Bridge
<p align="center">
<img src="https://www.tdgulf.com/wp-content/uploads/2016/02/1Pcs-HC-05-Serial-Transceiver-Wireless-Bluetooth-Module-A-Master-slave-Machine-Communication-Power-3-3V.jpg" width="275" height="275">
<p>
Additionally, we're using a bluetooth bridge to communicate with a secondary MCU via UART interface, to take input from an Android user via the bluetooth, and send it to the secondary MCU via the asynchronous link. Then, the secondary MCU transmit the user input to the primary MCU via a synchronous serial link using SPI.

## Software Platforms
* Keil uVision5
* STM32CubeMX


## Initial Assumptions
* The map of the gallery is predefined and positions of exhibits .
* The human interaction partners follow the robot’s suggestions without the need for the robot to check for their compliance.

## Challenges and Modifications 
1. We decided not to proceed working with Loomo Segway for number of reasons. These include the difficulty to deploy native C++ robotics applications on Loomo robot. The only Repository found that works as a bridge between JAVA SDK and C++ interfaces was in beta phase with no sufficient documentation and number of deprecated modules/libraries.
2. There were no suffecient resources that explain interfacing between the old color sensor (TCS3200) and the STM32 Nucleo Board (L432KC). Thus, we decided to replace it with another color sensor, namely TCS34725, which is performing noticeably better than the first one and has better documentations.


## Milestones
1. First Milestone (Following a Given Map and MP3 Module Implementation) DONE: Using a previously known localization map, the Dugo Thumper can guide the users and give them information about different exhibits, following a predefined path from start to end. 
2. Second Milestone (Art pieces detection & Information Retrieval) DONE: We will utilize the color sensor to detect the exhibits and then retrieve the related information. 
3. Third Milestone (Point-to-Point Mapping & User input taking) DONE: In this milestone, we shall add the feature of the robot being able to direct the users to their desired art piece from a any point in the exhibition. The User shall send the name of the art piece to the kit trough Bluetooth Module. 



  
## How to Build and Run
 To build this project clone both the Slave and Master projects. Upload the slave projcet on our primary Nucleo board and the Master project on the secodary Nucleo board. 
  
  Follow this Cube diagram here for the primary Nucleo board connections: 
  <p align="center">
  <img src="https://github.com/HadeelMabrouk/Embedded-Project-2/blob/main/Images/primary.png" width="575" height="575">
   <p>
  
  Follow this Cube diagram for the secondary Nucleo board connections: 
     <p align="center">
    <img src="https://github.com/HadeelMabrouk/Embedded-Project-2/blob/main/Images/secondary.png" width="575" height="475">
 <p>
  
## References
* [TCS3200 Color Sensor Tutorial](https://randomnerdtutorials.com/arduino-color-sensor-tcs230-tcs3200/)
* [TCS3200 Color Sensor Datasheet](https://drive.google.com/file/d/1-g4x6M_L4UjA4q5OPvafdh0qgD8-i72m/view?usp=sharing)
* [MP3 Mini Player Datasheet](http://www.electronicoscaldas.com/datasheet/DFR0299-DFPlayer-Mini-Manual.pdf)
* [MP3 Mini Player Tutorial](https://www.youtube.com/watch?v=FoB_49eAvFA)
* [TCS34725 Color Sensor Datasheet](https://cdn-shop.adafruit.com/datasheets/TCS34725.pdf)  
* [Adafruit TCS34725 Library](https://github.com/adafruit/Adafruit_TCS34725)  
  


## Proposal Video
Kindly, find the proposal video at [this link](https://drive.google.com/file/d/1foOe66EDGX7r4gehLcNGmYwh58q4fUl_/view?usp=sharing).  
For the slides, check [this link](https://drive.google.com/file/d/1M-g5JhZSnqB3LKiKBddfGtiu3rjULKN2/view?usp=sharing).

## First Demo Video
Kindly, find the demo for the first milestone at [this link](https://drive.google.com/file/d/1uGxQUxJxeEkoPAEkJR61Si_i3kNQVQKg/view?usp=sharing).
Find the MS1 presentation video at [this link](https://drive.google.com/file/d/1wY4F06E7ytnqnMiFXSs2j7YH9igndq4_/view?usp=sharing).  
For the slides, check [this link](https://drive.google.com/file/d/16R0G9x_ihUdd-awvE7O7YBITOf0AfcLg/view?usp=sharing).


## Team Members
* Hadeel Mabrouk - 900163213
* Israa Fahmy - 900171831
* Samah Ayman - 900171848
