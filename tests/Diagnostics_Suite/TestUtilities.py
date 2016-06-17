#! /usr/bin/env python

import numpy as np
from mecode import G
from time import sleep
import re
import os

class TestRunner:

    def __init__(self, logger, comport):
        '''Initialize the printer serial communication, and test variables'''
        self.g = G(
            print_lines=False,
            aerotech_include=False,
            direct_write=True,
            direct_write_mode='serial',
            # printer_port="/dev/tty.usbmodem1421",
            printer_port=comport,
        )
        self.logging = logger
        self.testFailed = False
        self.testFailCount = 0
        self.testSuccessCount = 0
        self.warningCount = 0
        self.errorList = []

        self.logging.debug("Test initialized")

    def TearDown(self):
        '''Save the printer state before the test ends'''
        self.g.write('M400', resp_needed=True)
        self.g.teardown()
        self.g = None

        self.logging.debug("Test Teardown Complete")

    def logFailure(self, message):
        '''Display the correct error message, increase the failure count,
            set the test to failed'''
        self.logging.error(message)
        self.testFailCount = self.testFailCount + 1
        self.testFailed = True
        self.errorList.append(message)

    def logWarning(self, message):
        '''Display the warning message, increase the warning count'''
        self.logging.warning(message)
        self.warningCount = self.warningCount + 1

    def logSuccess(self, message):
        '''Display the success message, increase the success count'''
        self.logging.info(message)
        self.testSuccessCount = self.testSuccessCount + 1

    def FinalDisplay(self):
        """Display final test data

        Display the final counts for passed, failed, and warnings, with
        test fail or pass

        """
        self.logging.info("")
        self.logging.info("----------------------------------------------")
        self.logging.info("Final Test Results")
        self.logging.info("----------------------------------------------")
        self.logging.info("Tests Passed: {0}".format(self.testSuccessCount))
        self.logging.info("Tests Failed: {0}".format(self.testFailCount))
        self.logging.info("Test Warnings: {0}".format(self.warningCount))
        self.logging.info("")
        if self.testFailed:
            self.logging.error("Test Failed")
        else:
            self.logging.info("Test Passed")
        self.logging.info("")
        self.logging.info("Error List")
        self.logging.info("----------------------------------------------")
        for i in range(len(self.errorList)):
            self.logging.error("{}".format(self.errorList[i]))

    def runTest(self, f, speed = "slow"):
        self.logging.info("Executing Test: {}".format(f.__name__))
        if speed is 'fast':
            returnvalue = f('fast')
        else:
            returnvalue = f()
        self.logging.debug("Return Value: {}".format(returnvalue.failed))
        if returnvalue.failed is False:
            self.logSuccess("Passed!\n")
        else:
            self.logFailure(returnvalue.data)
        self.logging.debug("Return Value: {}\n".format(returnvalue.data))


class ResponseData:
    def __init__(self):
        '''Pass through success or failure, and variables'''
        self.failed = False
        self.data = None

    def fail(self, message):
        self.failed = True
        self.data = message

    def success(self, message):
        self.failed = False
        self.data = message

class cd:
    """Context manager for changing the current working directory"""
    def __init__(self, newPath):
        self.newPath = os.path.expanduser(newPath)

    def __enter__(self):
        self.savedPath = os.getcwd()
        os.chdir(self.newPath)

    def __exit__(self, etype, value, traceback):
        os.chdir(self.savedPath)