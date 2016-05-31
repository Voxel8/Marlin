#! /usr/bin/env python

import numpy as np
from mecode import G
from time import sleep
import re
from colorlog import ColoredFormatter
import logging
import argparse

import sys
sys.path.append("./Diagnostics_Suite")
from TestUtilities import TestRunner, ResponseData
from testfunctions_i2c import I2C_Test
from testfunctions_pneumatics import Pneumatics_Test

###################################################################################################
"""Set up for the test framework, defining parser arguments and color logging"""

parser = argparse.ArgumentParser(
    description='A script for testing the I2C communications of a Voxel8 Printer'
)

parser.add_argument(dest = 'comport', 
                    metavar='N',
                    help='Target com port')

parser.add_argument(
    '-v', '--verbose',
    help="Increase verbosity",
    action="store_const", dest="loglevel", const=logging.DEBUG,
    default=logging.INFO,
)
args = parser.parse_args()   

logging.basicConfig(level=logging.DEBUG,  # DEBUG, INFO, WARNING
                    format='%(asctime)s %(levelname)-8s %(message)s',
                    datefmt='%d-%m-%y %H:%M:%S',
                    filename='console.log',
                    filemode='w'
                    )

console = logging.StreamHandler()

console.setLevel(args.loglevel)

formatter = ColoredFormatter(
    "%(black,bg_white)s%(asctime)s%(reset)s %(log_color)s%(levelname)-8s%(reset)s %(white)s%(message)s",
    datefmt='%d-%m-%y %H:%M:%S',
    reset=True,
    log_colors={
        'DEBUG':    'cyan',
        'INFO':     'green',
        'WARNING':  'yellow',
        'ERROR':    'red',
        'CRITICAL': 'white,bg_red',
    }
)
console.setFormatter(formatter)
logging.getLogger('').addHandler(console)

logging.debug("Logging and Parser Complete")


############################################################################################
## Test Main
############################################################################################

test = TestRunner(logging, args.comport)

I2C = I2C_Test(test, logging)
# test.runTest(I2C.test_i2c_alltargets)
# I2C.run_all_tests()

Pneumatics = Pneumatics_Test(test,logging)
Pneumatics.run_all_tests()


test.FinalDisplay()
test.TearDown()