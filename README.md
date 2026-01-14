# BirthDayApp

Save your friends' birthdays and be reminded of them with notifications.

## Features

- **Store Birthdays**: Efficiently store birthdays in a binary format.
- **Reminders**: Get desktop notifications for birthdays.
- **CLI Interface**: Simple command-line interface to add, delete, and list birthdays.
- **Automated**: Daily cron job checks for birthdays at 10:00 AM and 6:00 PM.

## Installation

### Dependencies

**Arch Linux:**
```bash
sudo pacman -S libnotify cmake make gcc
```

**Ubuntu/Debian:**
```bash
sudo apt install libnotify-dev cmake make gcc
```

### Install Script

The easiest way to install is using the provided script. This will build the application, install it to `~/.local/bin`, and set up the cron job.

```bash
./install.sh
```

### Manual Build

If you prefer to build manually:

```bash
mkdir -p build
cd build
cmake ..
make
```

## Usage

The application stores data in `$XDG_DATA_HOME/BirthDayApp/birthdays.dat` (usually `~/.local/share/BirthDayApp/birthdays.dat`).

### Add a Birthday
```bash
BirthDay -i "DD|MM|NAME|SURNAME"
```
Example:
```bash
BirthDay -i "15|01|John|Doe"
```

### Delete a Birthday
```bash
BirthDay -d "DD|MM|NAME|SURNAME"
```

### List Birthdays
List all birthdays:
```bash
BirthDay -l
```
List specific birthday:
```bash
BirthDay -l "DD|MM|NAME|SURNAME"
```

### Check for Birthdays (Notification)
This is usually run by the cron job, but you can run it manually:
```bash
BirthDay -s
```

## Project Structure

The project is modularized into:
- `main.c`: Entry point and CLI handling.
- `person.c/h`: Person data structure.
- `day.c/h`: Day data structure.
- `heap.c/h`: Min-heap implementation for efficient retrieval.
- `storage.c/h`: Binary storage logic.
- `utils.c/h`: Helper functions.

---

Happy coding!
