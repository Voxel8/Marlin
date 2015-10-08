#!/bin/bash
HERE=$(pwd)
sh $HERE/version.sh . Marlin/_Version.h
case "$(uname -s)"
  in Darwin)
    ARDUINO_EXEC="~/Applications/Arduino.app"
    ARDUINO_DEP="~/Applications/Arduino.app/Contents/Java/hardware/arduino/avr/"
  ;; Linux)
    echo 'Linux'
  ;; CYGWIN*|MINGW32*|MSYS*)
    ARDUINO_EXEC="C:/Program\ Files\ \(x86\)/Arduino/arduino_debug.exe --verify $HERE/Marlin/Marlin.ino --pref build.path=$HERE/build/ --pref board=rambo --port $1"
    ARDUINO_DEP="C:/Program Files (x86)/Arduino/hardware/arduino/avr"
  ;; *)
    echo 'This operating system is unfamiliar'
  ;;
esac
# Prepare for build by copying in RAMBo boards.txt and pins Files

if [[ -f "$ARDUINO_DEP/boards.txt" ]]; then
    echo "exists"
fi
if [ -d "$ARDUINO_DEP" ]; then
  # Control will enter here if $DIRECTORY exists.
  echo "Directory exists"
  eval "mv \"$ARDUINO_DEP/variants/rambo/\" \"$ARDUINO_DEP/variants/rambo_backup/\""
fi
# Even if a user doesn't have a rambo folder, they should have a boards.txt
eval "mv \"$ARDUINO_DEP/boards.txt\" \"$ARDUINO_DEP/boards_backup.txt\""
eval "cp \"$HERE/ArduinoAddons/Arduino_1.5.x/hardware/marlin/avr/boards.txt\" \"$ARDUINO_DEP/\""
eval "cp -r \"$HERE/ArduinoAddons/Arduino_1.5.x/hardware/marlin/avr/variants/rambo/\" \"$ARDUINO_DEP/variants/rambo/\""

# Create the build directory
mkdir $HERE/build/
eval $ARDUINO_EXEC
case "$?" in
  0) echo "Your sketch was successfully built and uploaded."
  ;;
  1) echo "There was an error building or uploading your sketch."
  ;;
  2) echo "The desired sketch was not found."
  ;;
  3) echo "Invalid argument option."
  ;;
  4) echo "The desired preference option does not exist."
  ;;
esac
# C:/Program\ Files\ \(x86\)/Arduino/hardware/tools/avr/bin/avrdude.exe -CC:/Program\ Files\ \(x86\)/Arduino/hardware/tools/avr/etc/avrdude.conf -patmega2560 -cwiring -PCOM4 -b115200 -D -Uflash:w:./build/Marlin.cpp.hex:i

# Clean Up
rm -rf ./build/
if [ -d "$ARDUINO_DEP/variants/rambo_backup" ]; then
eval "rm \"$ARDUINO_DEP/boards.txt\""
eval "rm -rf \"$ARDUINO_DEP/variants/rambo/\""
eval "mv \"$ARDUINO_DEP/boards_backup.txt\" \"$ARDUINO_DEP/boards.txt\""
eval "mv \"$ARDUINO_DEP/variants/rambo_backup/\" \"$ARDUINO_DEP/variants/rambo/\""
fi
