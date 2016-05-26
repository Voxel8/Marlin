from mecode import G
import numpy as np
import logging
import re
from time import sleep
from TestUtilities import ResponseData


class I2C_Test:

    def __init__(self, test, logging):
        """Initialize the I2C test, with the supplied overall test and logger.

        Args:
            test (testRunner): The testrunner used to call the I2C Test,
                allowing the use of the g code connection and the testrunner for
                run_all_tests
            logging (logger): Allows whatever preconfigured logging module used
                in the main program to be accessed in this file..

        """
        self.g = test.g
        self.test = test

        self.laserHysteresis = 50
        self.laserHeightDifferenceAcceptedRange = 400
        self.laserMax = 27700
        self.laserObservedDifference = 10700
        self.adcMax = 10300

        logging.debug("Initialized I2C Test")

###############################################################################
## Utility Functions
###############################################################################

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
        u = ResponseData()

        resp = self.g.write(
            command + ' ' + p1 + ' ' + p2 + ' ' + p3, resp_needed=True)
        logging.debug(
            str(command + ' ' + p1 + ' ' + p2 + ' ' + p3 + '  Response: ' + resp))

        PacketSplit = re.findall('Packet', resp)
        TimeoutSplit = re.findall('Timeout', resp)

        if len(PacketSplit):
            u.fail("Dropped " + str(len(PacketSplit)) + " Packets from address " + str(
                p1) + ". " + str(p1) + " is either not present or not communicating.")
        if len(TimeoutSplit):
            u.fail(str(len(TimeoutSplit)) + " Timeouts from " + str(p1))
        u.success(resp)
        return u

    def parseCartridgeStatus(self, cartridgeStatus, cartridgeNumber):
        """ Checks the cartridge status

        Parses and dispalys the cartridge status, and then checks to see if the
        flash and eeprom have been loaded correctly, based on the firmware
        version and the cartridge type.

        Args:
            cartridgeStatus (string): The string to be parsed
            cartridgeNumber (string): Which cartridge it is
                ("Cartridge 0", "Cartridge 1", or "Cartridge Holder")

        Returns:
            None

        """
        u = ResponseData()
        infoList = re.split('\W+', cartridgeStatus)
        try:
            if (len(infoList) >= 18):
                logging.info("%s Info", cartridgeNumber)
                logging.debug(infoList)
                logging.debug("-------------------------------------")
                logging.debug("Cartridge Serial Number: %s", infoList[2])
                logging.debug("Cartridge Firmware Version: %s", infoList[18])
                logging.debug("Cartridge Programmer Station: %s", infoList[5])
                logging.debug("Cartridge Type: %s", infoList[8])
                logging.debug("Cartridge Size: %s", infoList[11])
                logging.debug("Cartridge Material: %s", infoList[14])
    
                if (int(infoList[18]) == 255):
                    u.fail(
                        "{} FLASH not loaded properly, or cartridge is out of date\n".format(cartridgeNumber))
                    return u
    
                if (int(infoList[8]) == 15) and (cartridgeNumber != "Cartridge Holder"):
                    u.fail(
                        "{} EEPROM not loaded properly\n".format(cartridgeNumber))
    
                u.data = "Success"
                return u
        except:
            u.fail("{} Status Parse Failed. Received Message: {}".format(cartridgeNumber, cartridgeStatus))
            return u

    def readLaser(self):
        """ Return the laser reading

        Queries the laser value, which is sent to Marlin by I2C.

        Args:
            None

        Returns:
            The value of the laser

        """
        u = ResponseData()
        logging.info("Reading Laser")
        M234Response = self.g.write("M234", resp_needed=True).split()
        logging.debug(M234Response)
        if int(M234Response[1]) > self.laserMax:
            u.fail("Too far")
            return u
        else:
            u.data = int(M234Response[1])
            return u

################################################################################
## Test Functions
################################################################################

    def run_all_tests(self):
        self.test.runTest(self.test_i2c_alltargets)
        self.test.runTest(self.test_cart0_info)
        self.test.runTest(self.test_cart1_info)
        self.test.runTest(self.test_cartridgeholder)
        self.test.runTest(self.test_laser)

    def test_i2c_alltargets(self):
        """ Queries each cartridge and cartridge holder for each EEPROM spot,
            expecting a response

        Conditions:
            Pass: Each target gives an accurate EEPROM response
            Fail: Any timeouts or dropped packets from any target

        """
        t = ResponseData()
        logging.info(
            "Querying first 30 EEPROM locations for each cartridge and the cartridge holder")
        for i in range(0, 30):
            string = 'E'+str(i)
            response = self.commandparser('M244', 'C0', string)
            if response.failed:
                return response
            response = self.commandparser('M244', 'C1', string)
            if response.failed:
                return response
            response = self.commandparser('M244', 'C2', string)
            if response.failed:
                return response
        t.success("All EEPROM locations successfully queried\n")
        return t

    def test_cart0_info(self):
        """ Retrieves cartridge information from Cartridge 0

        Conditions:
            Pass: Gives a response to M245
            Fail: Any timeouts or dropped packets

        """
        t = ResponseData()
        logging.info("Retrieving Cartridge 0 information")
        cart0status = self.commandparser('M245', 'C0')
        if cart0status.failed:
            return cart0status
        parsedStatus = self.parseCartridgeStatus(cart0status.data, "Cartridge 0")
        if parsedStatus.failed:
            return parsedStatus
        t.success("Cartridge 0 info retrieved successfully\n")
        return t    


    def test_cart1_info(self):
        """ Retrieves cartridge information from Cartridge 1

        Conditions:
            Pass: Gives a response to M245
            Fail: Any timeouts or dropped packets

        """
        t = ResponseData()
        logging.info("Retrieving Cartridge 1 information\n")
        cart1status = self.commandparser('M245', 'C1')
        if cart1status.failed:
            return cart1status
        parsedStatus = self.parseCartridgeStatus(cart1status.data, "Cartridge 1")
        if parsedStatus.failed:
            return parsedStatus
        t.success("Cartridge 1 info retrieved successfully\n")
        return t    

    def test_cartridgeholder(self):
        """ Retrieves eeprom information from Cartridge Holder

        Conditions:
            Pass: Gives a response to M245
            Fail: Any timeouts or dropped packets

        """
        t = ResponseData()
        logging.info("Retrieving Cartridge Holder information\n")
        cartholderstatus = self.commandparser('M245', 'C2')
        if cartholderstatus.failed:
            return cartholderstatus
        parsedStatus = self.parseCartridgeStatus(cartholderstatus.data, "Cartridge Holder")
        if parsedStatus.failed:
            return parsedStatus

        t.success("Cartridge Holder info retrieved successfully\n")
        return t    

    def test_laser(self):
        """ Tests the laser responses to see that the ADC and laser are
            responding correctly.

        Conditions:
            Pass: After each movement, laser gives a valid response and stays
                within bounds of the test.
            Fail: Any of the responses given are outside the set limits of the
                test.

        """
        t = ResponseData()
        logging.info("Testing Laser Response")
        logging.info("Setting Printer to Relative Mode:")
        relativeResponse = self.g.write("G91", resp_needed=True)
        logging.debug(relativeResponse)
        logging.info("Homing to put bed in range for laser")
        homingresponse = self.g.write("G28", resp_needed=True)
        logging.debug(homingresponse)
        logging.info("Homing Complete")

        M234ResponseHigh1 = self.readLaser()
        if M234ResponseHigh1.failed:
            return M234ResponseHigh1

        logging.info("Reading ADC")
        M235Response = self.g.write("M235", resp_needed=True).split()
        if int(M235Response[1]) > self.adcMax:
            t.fail("Too far, out of adc max range")
            return t

        logging.debug(M235Response)
        logging.info("Moving Z")
        G0Response = self.g.write(
            "G0 Z4 F10000.0", resp_needed=True).split()
        logging.debug(G0Response)
        sleep(1)

        M234ResponseLow = self.readLaser()
        if M234ResponseLow.failed:
            return M234ResponseLow

        observed_diff = self.laserObservedDifference
        diff = M234ResponseHigh1.data - M234ResponseLow.data
        logging.debug("Distance Difference Pre and Post: {0}".format(diff))
        if diff > observed_diff + self.laserHeightDifferenceAcceptedRange or diff < observed_diff - self.laserHeightDifferenceAcceptedRange:
            t.fail("Distance between original reading and new reading is too large")
            return t

        logging.info("Moving Z")
        G0Response = self.g.write(
            "G0 Z-4 F10000.0", resp_needed=True).split()
        logging.debug(G0Response)

        sleep(1)

        M234ResponseHigh2 = self.readLaser()
        if M234ResponseHigh2.failed:
            return M234ResponseHigh2

        diff = M234ResponseHigh1.data - M234ResponseHigh2.data
        logging.debug("Distance Difference High 1 and 2: {0}".format(diff))
        if diff > self.laserHysteresis or diff < -self.laserHysteresis:
            t.fail("Too far")
            return t

        
        t.success("Laser Test Passed\n")
        return t
