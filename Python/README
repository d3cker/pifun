DHT11 humidity and temperature sensor
#####################################
Connected to:

+5VCC, Mass , GPIO4 data
http://www.circuitbasics.com/how-to-set-up-the-dht11-humidity-sensor-on-the-raspberry-pi/



1) For C (skip if Python only)

sudo apt-get update
sudo apt-get upgrade
sudo apt-get install git-core
git clone git://git.drogon.net/wiringPi
cd wiringPi
./build



2) For Python

git clone https://github.com/adafruit/Adafruit_Python_DHT.git
cd Adafruit_Python_DHT
sudo apt-get install build-essential python-dev
sudo python setup.py install


Adafruit OLED mini display 128x32
#################################

https://learn.adafruit.com/adafruit-pioled-128x32-mini-oled-for-raspberry-pi

Requirements:

1) Install soft

sudo apt-get update
sudo apt-get install build-essential python-dev python-pip
sudo pip install RPi.GPIO
sudo apt-get install python-imaging python-smbus

sudo apt-get install git
git clone https://github.com/adafruit/Adafruit_Python_SSD1306.git
cd Adafruit_Python_SSD1306
sudo python setup.py install


2) enable I2C

sudo apt-get install -y python-smbus
sudo apt-get install -y i2c-tools

sudo raspi-config -> Advanced -> I2C


sudo nano /etc/modules
add: 
i2c-bcm2708
i2c-dev

reboot

sudo i2cdetect -y 1

