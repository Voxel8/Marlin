#! /usr/bin/env python

import numpy as np
from mecode import G
from time import sleep
import re
from colorlog import ColoredFormatter
import logging
import argparse

from testfunctions_i2c import I2C_Test
from testfunctions_ereg import Ereg_Test

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

class Test:

    def __init__(self):
        '''Initialize the printer serial communication, and test variables'''
        self.g = G(
            print_lines=False,
            aerotech_include=False,
            direct_write=True,
            direct_write_mode='serial',
            # printer_port="/dev/tty.usbmodem1421",
            printer_port=args.comport,
        )

        self.testFailed = False
        self.testFailCount = 0
        self.testSuccessCount = 0
        self.warningCount = 0

        logging.debug("Test initialized")

    def TearDown(self):
        '''Save the printer state before the test ends'''
        self.g.write('M400', resp_needed=True)
        self.g.teardown()
        self.g = None

        logging.debug("Test Teardown Complete")

    def logFailure(self,message):
        '''Display the correct error message, increase the failure count, set the 
            test to failed'''
        logging.error(message)
        self.testFailCount = self.testFailCount + 1
        self.testFailed = True

    def logWarning(self,message):
        '''Display the warning message, increase the warning count'''
        logging.warning(message)
        self.testFailCount = self.warningCount + 1

    def logSuccess(self,message):
        '''Display the success message, increase the success count'''
        logging.info(message)
        self.testSuccessCount = self.testSuccessCount + 1

    def FinalDisplay(self):
        '''Display the final counts for passed, failed, and warnings, with test fail or pass'''
        logging.info("")
        logging.info("----------------------------------------------")
        logging.info("Final Test Results")
        logging.info("----------------------------------------------")
        logging.info("Tests Passed: {0}".format(self.testSuccessCount))
        logging.info("Tests Failed: {0}".format(self.testFailCount))
        logging.info("Test Warnings: {0}".format(self.warningCount))
        logging.info("")
        if self.testFailed:
            logging.error("Test Failed")
        else:
            logging.info("Test Passed")
        logging.info("")

############################################################################################
## Test Main
############################################################################################

test = Test()

I2C = I2C_Test(test,logging)
I2C.run_all_tests()

Ereg = Ereg_Test(test,logging)

test.FinalDisplay()
test.TearDown()