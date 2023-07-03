# BirthDayApp

Works only on Ubuntu-like OS.

Save your friends' birthdays and be reminded of them with notifications.

- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)

## Features

This app has the following features: inserting/deleting new birthdays, listing specific or all saved birthdays, and notifying upcoming birthdays.

## Installation

First, you need to install the required dependencies: `libnotify-dev` and `cmake`. Open a terminal and run the following commands:

    sudo apt install libnotify-dev
    sudo apt install cmake

Next, navigate to the downloaded repository and build the project using CMake:

    cmake -S . -B build
    cd build && make

To schedule the notifications, open the crontab editor:

    crontab -e

Insert the following line in the crontab file, replacing `/path/to/your/program/build/` with the actual path to your program's build directory:

    0 */8 * * * export DISPLAY=:0 && export XDG_RUNTIME_DIR=/run/user/$(id -u) && cd /path/to/your/program/build/ && ./BirthDay -s >> ./log.txt 2>&1

## Usage

To insert a birthday, use the following command:

    ./BirthDay -i "DAY|MONTH|NAME|SURNAME"

To delete a birthday, use the following command:

    ./BirthDay -d "DAY|MONTH|NAME|SURNAME"

To trigger the notification, use the following command:

    ./BirthDay -s

---

Happy coding! If you need any further assistance, feel free to ask.
