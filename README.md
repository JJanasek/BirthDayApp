# BirthDayApp

Works only on Ubuntu-like OS.
Save your friends' birthdays and be reminded of them with notifications.

Provide
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)

## Features

This app has following features: inserting/deleting new birthday, listing specific or all saved birthday and notifies the soones birthday.

## Installation
Firstly you need to install this libnotify and cmake: 
'''
sudo apt install libnotify-dev
sudo apt install cmake
'''
change directory to downloded repo and run:

'''
cmake -S . -Bbuild
cd build && make
'''

then run:
'''
crontab -e
'''
and insert this line:
'''
0 8 * * * export DISPLAY=:0 && export XDG_RUNTIME_DIR=/run/user/$(id -u) && cd /path/to/your/program/buid/ && ./BirthDay -s >> ./log.txt 2>&1

'''
where "/path/to/your/program/" change to directory where directory with build.

## Usage

to insert birthday use:

'''
./BirthDay -i "DAY|MONTH|NAME|SURNAME"
'''

to delete birthday use:

'''
./BirthDay -d "DAY|MONTH|NAME|SURNAME"
'''

to bring notification use:

'''

./BirthDay -s

'''


---

Feel free to modify this template to fit the specifics of your project. Make sure to provide clear and concise information so that users can quickly understand and use your project.

Happy coding! If you need any further assistance, feel free to ask.
