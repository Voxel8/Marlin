from mecode import G
import numpy as np
import logging
import re
from time import sleep


class I2C_Test:

    def __init__(self, test, logging):
        """Initialize the I2C test, with the supplied overall test and logger.

        Args:
            test (testRunner): The testrunner used to call the I2C Test, 
                allowing the use of test failing / passing / warnings.
            logging (logger): Allows whatever preconfigured logging module used
                in the main program to be accessed in this file..
        
        """
        self.g = test.g
        self.test = test
        self.testFailed = False
        self.laserHysteresis = 50
        self.laserMax = 27700
        self.laserObservedDifference = 10700
        self.adcMax = 10300

        logging.debug("Initialized I2C Test")

    def commandparser(self, command, p1='', p2='', p3=''):
        """ Checks for errors on sent commands, returns response.

        Args:
            command (string): The M or G code being sent
            p1 (string): first argument for the M Code
            p2 (string): second argument for the M Code
            p3 (string): third argument for the M Code

        Returns:
            int: The response sent from the printer. Returns 0 if the packet was
            dropped or if the request timed out.

        """
        resp = self.g.write(
            command + ' ' + p1 + ' ' + p2 + ' ' + p3, resp_needed=True)
        logging.debug(
            str(command + ' ' + p1 + ' ' + p2 + ' ' + p3 + '  Response: ' + resp))
        PacketSplit = re.findall('Packet', resp)
        TimeoutSplit = re.findall('Timeout', resp)
        if len(PacketSplit):
            self.test.logFailure("Dropped " + str(len(PacketSplit)) + " Packets from address " + str(
                p1) + ". " + str(p1) + " is either not present or not communicating.")
            resp = "0"
            self.testFailed = True
        if len(TimeoutSplit):
            self.test.logFailure(
                str(len(TimeoutSplit)) + " Timeouts from " + str(p1))
            resp = "0"
            self.testFailed = True
        return resp

    def parseCartridgeStatus(self, cartridgeStatus, cartridgeNumber):

        if (len(cartridgeStatus) >= 18):
            logging.info("%s Info", cartridgeNumber)
            infoList = re.split('\W+', cartridgeStatus)
            logging.debug(infoList)
            logging.debug("-------------------------------------")
            logging.debug("Cartridge Serial Number: %s", infoList[2])
            logging.debug("Cartridge Firmware Version: %s", infoList[18])
            logging.debug("Cartridge Programmer Station: %s", infoList[5])
            logging.debug("Cartridge Type: %s", infoList[8])
            logging.debug("Cartridge Size: %s", infoList[11])
            logging.debug("Cartridge Material: %s", infoList[14])

            if (int(infoList[18]) == 255):
                self.test.logFailure(
                    "%s FLASH not loaded properly, or cartridge is out of date\n", cartridgeNumber)
                self.testFailed = True
                logging.info(' ')

            if (int(infoList[8]) == 15) and (cartridgeNumber != "Cartridge Holder"):
                self.test.logFailure(
                    "%s EEPROM not loaded properly\n", cartridgeNumber)
                self.testFailed = True
                logging.info(' ')
            logging.info(' ')

    def readLaser(self):
        logging.info("Reading Laser")
        M234Response = self.g.write("M234", resp_needed=True).split()
        logging.debug(M234Response)
        if int(M234Response[1]) > self.laserMax:
            self.test.logFailure("Too far")
            self.testFailed = True
        return int(M234Response[1])

    def run_all_tests(self):
        self.test_i2c_alltargets()
        self.test_cart0_info()
        self.test_cart1_info()
        self.test_cartridgeholder()
        self.test_laser()

    def test_i2c_alltargets(self):
        logging.info(
            "Querying first 30 EEPROM locations for each cartridge and the cartridge holder")
        for i in range(0, 30):
            if self.testFailed == False:
                string = 'E'+str(i)
                self.commandparser('M244', 'C0', string)
                self.commandparser('M244', 'C1', string)
                self.commandparser('M244', 'C2', string)
        if self.testFailed == False:
            self.test.logSuccess("All EEPROM locations successfully queried\n")
        else:
            self.testFailed = False

    def test_cart0_info(self):
        logging.info("Retrieving Cartridge 0 information\n")
        cart0status = self.commandparser('M245', 'C0')
        self.parseCartridgeStatus(cart0status, "Cartridge 0")
        if self.testFailed == False:
            self.test.logSuccess("Cartridge 0 info retrieved successfully\n")
        else:
            self.testFailed = False

    def test_cart1_info(self):
        logging.info("Retrieving Cartridge 1 information\n")
        cart1status = self.commandparser('M245', 'C1')
        self.parseCartridgeStatus(cart1status, "Cartridge 1")
        if self.testFailed == False:
            self.test.logSuccess("Cartridge 1 info retrieved successfully\n")
        else:
            self.testFailed = False

    def test_cartridgeholder(self):
        logging.info("Retrieving Cartridge Holder information\n")
        cartridgeHolderStatus = self.commandparser('M245', 'C2')
        self.parseCartridgeStatus(cartridgeHolderStatus, "Cartridge Holder")
        if self.testFailed == False:
            self.test.logSuccess(
                "Cartridge Holder info retrieved successfully\n")
        else:
            self.testFailed = False

    def test_laser(self):
        logging.info("Testing Laser Response")
        logging.info("Homing to put bed in range for laser")
        homingresponse = self.g.write("G28", resp_needed=True)
        logging.debug(homingresponse)
        logging.info("Homing Complete")

        M234ResponseHigh1 = self.readLaser()

        if self.testFailed == False:
            logging.info("Reading ADC")
            M235Response = self.g.write("M235", resp_needed=True).split()
            if int(M235Response[1]) > self.adcMax:
                self.test.logFailure("Too far")
                self.testFailed = True
            logging.debug(M235Response)

            logging.info("Moving Z")
            G0Response = self.g.write(
                "G0 Z4 F10000.0", resp_needed=True).split()
            logging.debug(G0Response)

            sleep(1)

        if self.testFailed == False:
            M234ResponseLow = self.readLaser()

        if self.testFailed == False:
            observed_diff = self.laserObservedDifference
            diff = M234ResponseHigh1 - M234ResponseLow
            logging.debug("Distance Difference Pre and Post: {0}".format(diff))
            if diff > observed_diff + self.laserHysteresis or diff < observed_diff - self.laserHysteresis:
                self.test.logFailure("Too far")
                self.testFailed = True

        if self.testFailed == False:
            logging.info("Moving Z")
            G0Response = self.g.write(
                "G0 Z-4 F10000.0", resp_needed=True).split()
            logging.debug(G0Response)

            sleep(1)

            M234ResponseHigh2 = self.readLaser()

        if self.testFailed == False:
            diff = M234ResponseHigh1 - M234ResponseHigh2
            logging.debug("Distance Difference High 1 and 2: {0}".format(diff))
            if diff > self.laserHysteresis or diff < -self.laserHysteresis:
                self.test.logFailure("Too far")

        if self.testFailed == False:
            self.test.logSuccess("Laser Test Passed\n")
        else:
            self.testFailed = False
