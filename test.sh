#!/bin/bash
# HERE="/cygwin/c/Users/murphyk3/Documents/GitHub/Marlin"
# HERE=$(echo $HERE | sed 's/\/cygwin//g')
# echo $HERE

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
echo $ARDUINO_DEP
if [ -d "$ARDUINO_DEP" ]; then echo 'Exists'; else echo 'Not found'; fi