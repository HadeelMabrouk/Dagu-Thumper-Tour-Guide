
# Dagu Thumper Tour Guide
The main idea of this project is to design a conceptual robot tour guide for an art gallery using Dagu Thumper. The kit's task is to scan the art space, detect the exibits by detecting their color, recall background information and give the visitors a brief description about the history of the art piece, e.x. artist, period, location ..etc.

## Motivation 
Social robots has been recently used to serve multiple purposes, like personal assistance and performing tasks within the domain of domestic services and healthcare. Currently, there is a growing interest in the use of tour guide mobile robots in various environments such as museums, exhibitions and fairs, to provide help for the visitors by offering them a set of services, such as giving them general tours, helping them find points of interest, and providing information about different exhibits.

## Initial Design 
### Features
* Text-to-Speech: By Giving a Brief Description about an Art Piece.
* Color detection: To recognize the art piece through dtecting different colors.
* Obstacle Detection: To Avoid Collision.

### Logic Design 
The Kit will take the user from a starting point and guide them in the Exhibit hall. Using a previously known localization map, the Dugo Thumper can guide the users and give them information about different exhibits, following a predefined path from start to end using the color detector to detect any color changes. 

<p align="center">
<img src="https://github.com/HadeelMabrouk/Embedded-Project-2/blob/main/Images/logic.PNG" width="600" height="400">
<p>
  
### System Design
The following is a grahical representation of the full system design showing all the sensors and controllers. 
<p align="center">
<img src="https://github.com/HadeelMabrouk/Embedded-Project-2/blob/main/Images/system2.png" width="600" height="700">
<p>

### Physical Connections 
The following is a real representation of the whole system on top of the Dago Thumper kit. 
<p align="center">
<img src="https://github.com/HadeelMabrouk/Embedded-Project-2/blob/main/Images/thumper1.jpeg" width="800" height="600">
<p>
<p align="center">
<img src="https://github.com/HadeelMabrouk/Embedded-Project-2/blob/main/Images/thumper2.jpeg" width="800" height="1000">
<p>
  
## Hardware Components    
* The Ultrasonic sensor: in the front to detect obstacles in real time to prevent collisions.
* STM32 Nucleo Board (L432KC): to controle the communication between all the actuators. 
* Dagu Thumper
* Pololu TReX Motor Controller
* Color Sensor (TCS3200)
* MP3 Mini Player Module (MP3－TF－16P)
* MicroSD Memory Card
* Speakers

### Sensors 

#### Color Sensor

The TCS230 senses color light with the help of an 8 x 8 array of photodiodes. Then using a Current-to-Frequency Converter the readings from the photodiodes are converted into a square wave with a frequency directly proportional to the light intensity. Finally, using the Microcontroller Board we can read the square wave output and get the results for the color.

<b> How it Works </b>
* First set the input pins as input and output pins as output. No need to use analog pins.
* Set S0 and S1 to high or low to set desired frequency scaling.
* In loop, activate each filters by setting S2 and S3 to HIGH or LOW and measure frequency ‘fo’ from 6th pin to get corresponding colour intensity. Compare frequencies of each colour to determine the colour of the object.

Each 16 photodiodes are connected in parallel, so using the two control pins S2 and S3 we can select which of them will be read. So for example, if we want to detect red color, we can just use the 16 red filtered photodiodes by setting the two pins to low logic level according to the following table: 

<p align="center">
<img src="https://github.com/HadeelMabrouk/Embedded-Project-2/blob/main/Images/table1.png" width="500" height="140">
<p>

The sensor has two more control pins, S0 and S1 which are used for scaling the output frequency. The frequency can be scaled to three different preset values of 100 %, 20 % or 2%. This frequency-scaling function allows the output of the sensor to be optimized for various frequency counters or microcontrollers.

## Software Platform 
* Keil uVision5
* STM32CubeMX


## Initial Assumptions
* The map of the gallery is predefined and positions of exhibits .
* The human interaction partners follow the robot’s suggestions without the need for the robot to check for their compliance.

## Challenges and Modifications 
1. We decided not to proceed working with Loomo Segway for number of reasons. These includes the inability to deploy native C++ robotics applications on Loomo robot. The only Repository found hat works as a bridge between JAVA SDK and C++ interfaces was in alpha phase with no sufficient documentation and number of deprecated modules/libraries. 
2. The is no resources found that explain interfacing between our color sensor(TCS230) and our microcontroller (stm32l432kc). However, There are alot of support when using Arduino. So we were able to operate on Arduino to test our hypothesis . We used the sensor with stm32l432kc board, but the results are not very accurate. 

## Milestones
1. First Milestone (A Predefined Path Following & Speaking Features & obstacles detection): Using a previously known localization map, the Dugo Thumper can guide the users and give them information about different exhibits, following a predefined path from start to end along with avoiding any obstacle in its way. 
2. Second Milestone (Art pieces detection & Information Retrieval Feature): We will utilize the color sensor to detect the exhibits and then retrieve the related information. 
3. Third Milestone (Point-to-Point Mapping Feature): In this milestone, we shall add the feature of the robot being able to direct the users to their desired art piece from a any point in the exhibition. The User shall send the name of the art piece tp the kit trough Bluetooth Module. 


## References
* [TCS3200 Color sensor tutorial](https://randomnerdtutorials.com/arduino-color-sensor-tcs230-tcs3200/)
* [TCS3200 Color Sensor Datasheet](https://drive.google.com/file/d/1-g4x6M_L4UjA4q5OPvafdh0qgD8-i72m/view?usp=sharing)


## Proposal Video
For the project demo please follow [this link](project demo).  
For the slides, check [this link](slides).

## Team Members
* Hadeel Mabrouk - 900163213
* Israa Fahmy - 900171831
* Samah Ayman - 900171848
