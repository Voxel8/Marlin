#!/bin/bash
# Correct Syntax: ./build.sh [port [*upload | verify]]
set -e
HERE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$HERE"
OPERATING_SYSTEM="$(uname -s)"
VERSION='Voxel8 Build Debug'

# Begin build script
if [ -z "$1" ]; then
  # Since ino can automatically detect the serial port, an argument is not necessary on Linux
  if [ ! "$OPERATING_SYSTEM" = "Linux" ]; then
    echo "Please enter the port of the device or specify 'verify'"
    exit 1
  fi
fi
if [ "$1" = "upload" ]; then
  echo "Syntax error: The device port must be specified before the 'upload' argument."
  echo "Please use --help for more info."
  exit 1
elif [ "$1" = "verify" ]; then
  if [ ! "$OPERATING_SYSTEM" = "Linux" ]; then
    PORT_ARG=""
    COMMAND="--verify"
  fi
elif [ "$1" = "--help" ] && [ -z "$2" ]; then
  echo $VERSION
  echo "A star (*) represents a default value."
  echo ""
  echo "SYNTAX: build.sh [port [*upload | verify]]"
  echo ""
  echo "Type '--help name' to find out more about the argument 'name'."
  exit 1
elif [ "$1" = "--help" ] && [ ! -z "$2" ]; then
  if [ "$2" = "port" ]; then
    echo "This is the port of the device in which the sketch will be uploaded to. Can be omitted if just verifying."
  elif [ "$2"= "verify" ]; then
    echo "Specifying this argument will only compile the script. No uploading will be done."
  elif [ "$2" = "upload" ]; then
    echo "Specifying this argument will compile and upload the sketch to the given device port."
  else
    echo "This argument does not exist."
  fi
  exit 1
else
  if [ ! "$OPERATING_SYSTEM" = "Linux" ]; then
    PORT_ARG="--port $1"
    if [ -z "$2" ]; then
      COMMAND="--upload"
    elif [ "$2" = "verify" ] || [ "$2" = "upload" ]; then
      COMMAND="--$2"
    else
      echo "Invalid argument. Please use --help for more info."
      exit 1
    fi
  fi
fi

case "$OPERATING_SYSTEM"
  in Darwin)
    ARDUINO_EXEC="/Applications/Arduino.app/Contents/MacOS/Arduino $COMMAND \"$HERE/Marlin/Marlin.ino\" --pref build.path=\"$HERE/build/\" --pref board=rambo $PORT_ARG"
    ARDUINO_DEP="/Applications/Arduino.app/Contents/Java/hardware/arduino/avr"
  ;; Linux)
    ARDUINO_DEP="/usr/share/arduino/hardware/arduino"
    ARDUINO_EXEC_COMPILE="ino build -m mega2560"
    ARDUINO_EXEC_UPLOAD="ino upload -m mega2560"
  ;; CYGWIN*)
    CYGHERE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && cygpath -aw $(pwd) )"
    ARDUINO_EXEC="C:/Program\ Files\ \(x86\)/Arduino/arduino_debug.exe $COMMAND \"$CYGHERE/Marlin/Marlin.ino\" --pref build.path=\"$HERE/build/\" --pref board=rambo $PORT_ARG"
    ARDUINO_DEP="C:/Program Files (x86)/Arduino/hardware/arduino/avr"
  ;;MINGW32*|MINGW64*|MSYS*)
    ARDUINO_EXEC="C:/Program\ Files\ \(x86\)/Arduino/arduino_debug.exe $COMMAND \"$HERE/Marlin/Marlin.ino\" --pref build.path=\"$HERE/build/\" --pref board=rambo $PORT_ARG"
    ARDUINO_DEP="C:/Program Files (x86)/Arduino/hardware/arduino/avr"
  ;; *)
    echo 'This operating system is unfamiliar'
    exit 1
esac

echo $VERSION
echo ""
echo "Current location: $HERE"

# Check for build directory
if [ ! "$OPERATING_SYSTEM" = "Linux" ]; then
  if [ -d "$HERE/build/" ]; then
    echo "Build directory exists"
  fi
fi

echo "EVAL: $ARDUINO_EXEC"
echo ""
echo "DEP: $ARDUINO_DEP"