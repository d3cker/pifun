#!/bin/bash

echo -en "[*] bootstrap init\n\n"

#sudo apt update

# check for git

echo "[+] checking for git"
if [ ! -f /usr/bin/git ]; then
    sudo apt install git-core -y
else
    echo -en "[i] found\n\n"
fi

echo -en "[+] Adafruit DHT11 sensor python setup\n\n"

sudo apt install build-essential python-dev -y
git clone https://github.com/adafruit/Adafruit_Python_DHT.git
cd Adafruit_Python_DHT
sudo python setup.py install
cd -

echo -en "\n\n[*] OLED setup\n\n"

sudo apt install python-pip python-imaging python-smbus -y
sudo pip install RPi.GPIO
git clone https://github.com/adafruit/Adafruit_Python_SSD1306.git
cd Adafruit_Python_SSD1306
sudo python setup.py install
cd -

echo -en "\n\n[*] enable I2C support"

sudo apt install i2c-tools -y
