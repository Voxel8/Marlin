#!/bin/bash
# Correct Syntax: ./build.sh [port [*upload | verify]]

cleanup()
{
  if [ ! "$OPERATING_SYSTEM" = "Linux" ]; then
    rm -rf ./build/
    if [ -d "$ARDUINO_DEP/variants/rambo_backup" ]; then
      rm -rf "$ARDUINO_DEP/variants/rambo/"
      mv "$ARDUINO_DEP/variants/rambo_backup/" "$ARDUINO_DEP/variants/rambo/"
    fi
    if [ -f "$ARDUINO_DEP/boards_backup.txt" ]; then
      rm "$ARDUINO_DEP/boards.txt"
      mv "$ARDUINO_DEP/boards_backup.txt" "$ARDUINO_DEP/boards.txt"
    fi
  else
    if [ -f ./.build/mega2560/firmware.hex ]; then
      rm ./.build/mega2560/firmware.hex
    fi
    if [ -d "$ARDUINO_DEP/variants/standard_backup" ]; then
      sudo rm -rf "$ARDUINO_DEP/variants/standard/"
      sudo mv "$ARDUINO_DEP/variants/standard_backup/" "$ARDUINO_DEP/variants/standard/"
    fi
    if [ -f "$ARDUINO_DEP/boards_backup.txt" ]; then
      sudo rm "$ARDUINO_DEP/boards.txt"
      sudo mv "$ARDUINO_DEP/boards_backup.txt" "$ARDUINO_DEP/boards.txt"
    fi
    if [ -d "$HERE/Wire-backup" ]; then
      cd /usr/share/arduino/libraries
      sudo rm -rf Wire
      sudo mv "$HERE/Wire-backup" Wire
      cd "$HERE"
    fi
  fi
}

abort()
{
    cleanup
    echo "Build files cleaned up, exiting..." >&2
    exit 1
}

trap 'abort' 0

set -e
HERE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$HERE"
OPERATING_SYSTEM="$(uname -s)"
VERSION='Voxel8 Marlin Build Script v1.0'

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

# For Linux Users, make sure they have installed libraries and src directory
if [ "$OPERATING_SYSTEM" = "Linux" ]; then
  if [ ! -d "./src/" ]; then
    ln -s "$HERE/Marlin" "$HERE/src"
  fi
  mkdir ./libraries/
  if [ ! -d "/usr/share/arduino/libraries/LiquidCrystal_I2C" ]; then
    echo "Missing LiquidCrystal_I2C - Cloning..."
    cd ./libraries/
    git clone -q https://github.com/kiyoshigawa/LiquidCrystal_I2C.git
    cd "$HERE"
    rm -rf ./.build/
  fi
  if [ ! -d "/usr/share/arduino/libraries/LiquidTWI2" ]; then
    echo "Missing LiquidTWI2 - Cloning..."
    cd ./libraries/
    git clone -q https://github.com/lincomatic/LiquidTWI2.git
    cd "$HERE"
    rm -rf ./.build/
  fi
  if [ ! -d "/usr/share/arduino/libraries/U8glib" ]; then
    echo "Missing U8glib - Retrieving..."
    wget -nv "https://bintray.com/artifact/download/olikraus/u8glib/u8glib_arduino_v1.18.1.zip"
    sudo unzip -q u8glib_arduino_v1.18.1.zip -d /usr/share/arduino/libraries/
    rm u8glib_arduino_v1.18.1.zip
    rm -rf ./.build/
  fi
  if [ ! -d "/usr/share/arduino/libraries/L6470" ]; then
    echo "Missing L6470 - Retrieving..."
    sudo cp -r ./ArduinoAddons/Arduino_1.x.x/libraries/L6470 /usr/share/arduino/libraries/
    rm -rf ./.build/
  fi
  if [ ! -d "/usr/share/arduino/libraries/TMC26XStepper" ]; then
    echo "Missing TMC26XStepper - Retrieving..."
    sudo cp -r ./ArduinoAddons/Arduino_1.x.x/libraries/TMC26XStepper /usr/share/arduino/libraries/
    rm -rf ./.build/
  fi
  if [ -d "/usr/share/arduino/libraries/Robot_Control" ]; then
    sudo rm -rf /usr/share/arduino/libraries/Robot_Control
    rm -rf ./.build/
  fi
  cd ./libraries
  sudo cp -r . /usr/share/arduino/libraries/
  cd "$HERE"
  rm -rf ./libraries/
  cd /usr/share/arduino/libraries
  if [ -d "Wire" ]; then
    sudo mv Wire "$HERE/Wire-backup"
  fi
  sudo cp -r "$HERE/ArduinoAddons/Arduino_1.6.x/hardware/marlin/avr/libraries/Wire" Wire
  cd "$HERE"
fi

# Generate _Version.h using Git repo info
if ! git diff-index --quiet HEAD --; then
  VERSION_MODIFIED="-modified"
else
  VERSION_MODIFIED=""
fi

OUTFILE=./Marlin/_Version.h
echo "/* This file was automatically generated by: $VERSION" >"$OUTFILE"
echo " * This file should not be manually editted." >>"$OUTFILE"
echo " * It does not get committed to the repository." >>"$OUTFILE"
echo " */" >>"$OUTFILE"
echo "" >>"$OUTFILE"

echo "#define BUILD_UNIX_DATETIME" `date +%s` >>"$OUTFILE"
echo "#define STRING_DISTRIBUTION_DATE" `date '+"%Y-%m-%d %H:%M"'` >>"$OUTFILE"
( set +e
  BRANCH=`git symbolic-ref -q --short HEAD`
  if [ "x$BRANCH" == "x" ] ; then
    BRANCH=""
  elif [ "x$BRANCH" == "xDevelopment" ] ; then
    BRANCH=" dev"
  else
    BRANCH=" $BRANCH"
  fi
  VERSION=`git describe --tags --first-parent 2>/dev/null`
  HASH=`git rev-parse --short HEAD`
  VERSION_NUM=`git describe --tags --abbrev=0`
  VERSION_NUM=$(echo $VERSION_NUM | sed 's/-.*//g')
  VERSION_MIDDLE=$(echo $VERSION | sed "s/$VERSION_NUM-\(.*\)-g$HASH/\1/")
  VERSION_COMMITS=$(echo $VERSION_MIDDLE | sed "s/.*-//")
  BRANCH=$(echo $BRANCH | sed 's/\//\\\//g')
  if [ "x$VERSION" != "x" ] ; then
    echo "#define BUILD_VERSION_NUMERIC \"$VERSION_NUM\"" >>"$OUTFILE"
    echo "#define SHORT_BUILD_VERSION \"$VERSION_NUM $BRANCH$VERSION_MODIFIED\"" >>"$OUTFILE"
    echo "#define DETAILED_BUILD_VERSION \"$VERSION_NUM $BRANCH-$VERSION_COMMITS-$HASH$VERSION_MODIFIED\"" >>"$OUTFILE"
  else
    VERSION=`git describe --tags --first-parent --always 2>/dev/null`
    echo "#define BUILD_VERSION_NUMERIC \"$VERSION_NUM\"" >>"$OUTFILE"
    echo "#define SHORT_BUILD_VERSION \"$BRANCH$VERSION_MODIFIED\"" >>"$OUTFILE"
    echo "#define DETAILED_BUILD_VERSION \"${BRANCH}-$VERSION$VERSION_MODIFIED\"" >>"$OUTFILE"
  fi
  URL=`git config --local --get remote.origin.url | sed "sx.*github.com.xhttps://github.com/x" | sed "sx\.gitx/x"`
  if [ "x$URL" != "x" ] ; then
    echo "#define SOURCE_CODE_URL  \""$URL"\"" >>"$OUTFILE"
  fi
)

# If /build/ exists, remove.
if [ ! "$OPERATING_SYSTEM" = "Linux" ]; then
  if [ -d "$HERE/build/" ]; then
    echo "Build directory exists, removing..."
    rm -rf "$HERE/build/"
  fi
else
  if [ -f "$HERE/.build/mega2560/firmware.hex" ]; then
    echo "Firmware file exists, removing..."
    rm "$HERE/.build/mega2560/firmware.hex"
  fi
fi

echo $VERSION
echo ""

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

# Prepare for build by copying in RAMBo boards.txt and pins files
if [ ! "$OPERATING_SYSTEM" = "Linux" ]; then
  if [ -d "$ARDUINO_DEP/variants/rambo" ] && [ ! -d "$ARDUINO_DEP/variants/rambo_backup" ]; then
    mv "$ARDUINO_DEP/variants/rambo/" "$ARDUINO_DEP/variants/rambo_backup/"
  fi
  # Even if a user doesn't have a rambo folder, they should have a boards.txt
  if [ -f "$ARDUINO_DEP/boards.txt" ]; then
    mv "$ARDUINO_DEP/boards.txt" "$ARDUINO_DEP/boards_backup.txt"
  fi
  cp "$HERE/ArduinoAddons/Arduino_1.6.x/hardware/marlin/avr/boards.txt" "$ARDUINO_DEP/"
  cp -r "$HERE/ArduinoAddons/Arduino_1.6.x/hardware/marlin/avr/variants/rambo/." "$ARDUINO_DEP/variants/rambo/"
else
  if [ -d "$ARDUINO_DEP/variants/standard" ] && [ ! -d "$ARDUINO_DEP/variants/standard_backup" ]; then
    sudo mv "$ARDUINO_DEP/variants/standard/" "$ARDUINO_DEP/variants/standard_backup/"
  fi
  if [ -f "$ARDUINO_DEP/boards.txt" ]; then
    sudo mv "$ARDUINO_DEP/boards.txt" "$ARDUINO_DEP/boards_backup.txt"
  fi
  sudo cp "$HERE/ArduinoAddons/Arduino_1.0.x/hardware/rambo/boards.txt" "$ARDUINO_DEP/"
  sudo cp -r "$HERE/ArduinoAddons/Arduino_1.0.x/hardware/rambo/variants/standard/." "$ARDUINO_DEP/variants/standard/"
fi

# Create the build directory
if [ ! "$OPERATING_SYSTEM" = "Linux" ]; then
  mkdir "$HERE/build/"
  eval $ARDUINO_EXEC
else
  eval $ARDUINO_EXEC_COMPILE
  if [ ! "$1" = "verify" ]; then
    eval $ARDUINO_EXEC_UPLOAD
  fi
fi
case "$?" in
  0) echo "The action has been performed successfully."
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

# # Copy HEX file for keep-sake
if [ ! "$OPERATING_SYSTEM" = "Linux" ]; then
  cp "$HERE/build/Marlin.cpp.hex" "$HERE/firmware.hex"
fi

cleanup

trap : 0
