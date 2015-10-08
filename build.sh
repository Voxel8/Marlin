#!/bin/bash
case "$(uname -s)"
  in Darwin)
    echo 'Mac OS X'
  ;; Linux)
    echo 'Linux'
  ;; CYGWIN*|MINGW32*|MSYS*)
    HERE=$(pwd)
    mkdir $HERE/build/
    arduino_debug "$2" $HERE/Marlin/Marlin.ino --pref build.path=$HERE/build/ --pref board=rambo --port "$1"
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
    rm -rf ./build/
  ;; *)
    echo 'This operating system is unfamiliar'
  ;;
esac

sh $HERE/version.sh . _Version.h