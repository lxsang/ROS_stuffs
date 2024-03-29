# Turtlebot_web

This project give a web based app to view the robot map and send remote actions. This project also contains specific launch files (and other resources) to use this web application with the Turtlebot with a hokuyo laser.

## Getting Started

### Prerequisites
This install assumes that you have completed the previous tutorials: [TurtleBot Bringup](http://wiki.ros.org/turtlebot_bringup/Tutorials/indigo/TurtleBot%20Bringup), [PC Bringup](http://wiki.ros.org/turtlebot_bringup/Tutorials/indigo/PC%20Bringup). 

#### Installing ros dependance	
	sudo apt install ros-kinetic-rosbridge-server 

#### Installing web-server dependance	
	sudo apt install nodejs-legacy

#### Installing vmap dependance
	request to guillaume.lozenguez@imt-lille-douai.fr

#### Installing kinect video dependance (beta)
	sudo apt install ros-kinetic-web-video-server 

## Installing project
	cd <catkin_repo>/src
	wget https://github.com/CARMinesDouai/turtlebot_web/archive/master.zip
	unzip master.zip
	cd ..
	catkin_make
	rm src/master.zip
	
## How to use
First : 

	ssh <robot ip>
	rosrun turtlebot_web run.sh

Then :
    
    access to http://<robot ip>:8080
    
## Authors

* **Christopher Lievin** - *Initial work* - [git](https://github.com/lievin-christopher)
* **Guillaume Lozenguez** - *Project Manager* - [website](http://car.mines-douai.fr/guillaume/)
* **Luc Fabresse** - *Project Manager* - [git](https://github.com/LucFabresse)
* **Noury Bouraqadi** - *Project Manager* - [git](https://github.com/bouraqadi)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
