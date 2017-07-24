# This is the TOROB catkin workspace for ROS.

# Dependancy to SDL2:
sudo apt install --yes libsdl2-dev libsdl2-gfx-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-net-dev libsdl2-ttf-dev

# Compile for the first time :
bin/initialize

# Et...
echo '# ROS::Torob configuration' >> ~/.bashrc
echo 'source ~/torob_ws/devel/setup.sh' >> ~/.bashrc
source ~/.bashrc

