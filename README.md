
# Loomo Tour Guide
The main idea of this project is to design a conceptual robot tour guide for an art gallery using Segway Loomo. The robot's task is to take the name of the art piece the user is wishing to visit. Then, Loomo shall take them to the exhibit they asked to see, and give them a brief description about the history of the art piece, e.x. artist, period, location ..etc.

## Motivation 
Social robots has been recently used to serve multiple purposes, like personal assistance and performing tasks within the domain of domestic services and healthcare. Currently, there is a growing interest in the use of tour guide mobile robots in various environments such as museums, exhibitions and fairs, to provide help for the visitors by offering them a set of services, such as giving them general tours, helping them find points of interest, and providing information about different exhibits.

## Initial Design 
### Features
* Speech Recognition: e.x. "Go to art piece1", "Go to art piece2", "General tour".
* Text-to-Speech: By Giving a Brief Description about an Art Piece.
* Localization and Tracking.
* Obstacle Detection: To Avoid Collision.

### Logic Design
![Logic Design](https://i.imgur.com/Y2jsA25.png)

### System Design
![System Design](https://i.imgur.com/HXbgxBY.png)

## Hardware Platform   
In this project, we are planning to use Segway Loomo primarily making use of the following sensors and actuators, as shown in the figure:
### Sensors
* The microphone array: which is composed of 5 mics is used to localize voice source and reduce background noises during voice recognition.
* The Ultrasonic sensor: in the front to detect obstacles in real time to prevent collisions.
* Intel's RealSense module: to provide aligned RGB-Depth images at 30fps, for visual Simultaneous Localization and Mapping (SLAM).
* The main HD camera: which is mounted next to the robot face, with a wide-angle of 104 degrees FOV.
### Actuators
* Speakers
* Wheel Motors  
* LCD Screen: for output displaying and debugging purposes.   
![Hardware Platform](https://i.imgur.com/Ue1aABD.png) 


## Software Platform 
Since Loomo runs on the Android version 5.1 operating system, we will need to use Android Studio as a development tool.
we'll use the Loomo SDK mainly focusing on the:
* Speech SDK
* Vision SDK
* Locomotion (Base) SDK 

## Initial Assumptions
* The map of the gallery is predefined.
* The human interaction partners follow the robot’s suggestions without the need for the robot to check for their compliance.

## Milestones
1. First Milestone (A Predefined Path Following & Speaking Features): Using a previously known localization map, the robot can guide the users and give them information about different exhibits, following a predefined path from start to end. 
2. Second Milestone (Speech Recognition & Information Retrieval Feature): Adding human-robot interaction feature in the form of taking the art piece name and directing the users towards it from a predefined point.
3. Third Milestone (Point-to-Point Mapping Feature): In this milestone, we shall add the feature of the robot being able to direct the users to their desired art piece from a any point in the exhibition.
4. Fourth Milestone (Mapping & Localization Feature): By using the camera sensor and RealSense module.

## References
* Liebl, Johanna. “Segway Robot Loomo As A Tour Guide.” 
* Segway Robotics. [Segway Robot Loomo Documentation](https://developer.segwayrobotics.com/developer/documents/segway-robot-overview.html). 


## Proposal Video
Kindly, find the proposal video at [this link](https://drive.google.com/file/d/1foOe66EDGX7r4gehLcNGmYwh58q4fUl_/view?usp=sharing).  
For the slides, check [this link](https://drive.google.com/file/d/1M-g5JhZSnqB3LKiKBddfGtiu3rjULKN2/view?usp=sharing).

## Team Members
* Hadeel Mabrouk - 900163213
* Israa Fahmy - 900171831
* Samah Ayman - 900171848
