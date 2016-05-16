from mecode import G
import numpy as np
import logging
import re
from time import sleep

class I2C_Test:
    
    def __init__(self,test,logging):
        '''Initialize the I2C test, with the supplied overall test and logger'''
        self.g = test.g
        self.test = test
        self.testFailed = False;
        logging.debug("Initialized I2C Test")

    def commandparser(self, command, p1='', p2='', p3=''):
        resp = self.g.write(
            command + ' ' + p1 + ' ' + p2 + ' ' + p3, resp_needed=True)
        logging.debug(
            str(command + ' ' + p1 + ' ' + p2 + ' ' + p3 + '  Response: ' + resp))
        PacketSplit = re.findall('Packet', resp)
        TimeoutSplit = re.findall('Timeout', resp)
        if len(PacketSplit):
            self.test.logFailure("Dropped " + str(len(PacketSplit)) + " Packets from address " + str(p1) + ". " + str(p1) + " is either not present or not communicating.")
            resp = "0"
            self.testFailed = True
        if len(TimeoutSplit):
            self.test.logFailure(str(len(TimeoutSplit)) + " Timeouts from " + str(p1))
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
                self.test.logFailure("%s EEPROM not loaded properly\n", cartridgeNumber)
                self.testFailed = True
                logging.info(' ')
            logging.info(' ')


    def run_all_tests(self):
        self.test_i2c_alltargets()
        self.test_cart0_info()
        self.test_cart1_info()
        self.test_cartridgeholder()
        self.test_laser()


    def test_i2c_alltargets(self):
        logging.info("Querying first 30 EEPROM locations for each cartridge and the cartridge holder")
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
            self.test.logSuccess("Cartridge Holder info retrieved successfully\n")
        else:
            self.testFailed = False

    def test_laser(self):
        logging.info("Testing Laser Response")
        logging.info("Homing to put bed in range for laser")
        homingresponse = self.g.write("G28",resp_needed=True)
        logging.debug(homingresponse)
        logging.info("Homing Complete")
        
        logging.info("Reading Laser")
        M234Response = self.g.write("M234",resp_needed=True).split()
        logging.debug(M234Response)
        if int(M234Response[1]) > 27700:
            self.test.logFailure("Too far")
        else:
            self.test.logSuccess("Laser in Range")
        
        logging.info("Reading ADC")
        M235Response = self.g.write("M235",resp_needed=True).split()
        if int(M235Response[1]) > 10300:
            self.test.logFailure("Too far")
        else:
            self.test.logSuccess("ADC value valid")
        logging.debug(M235Response)

        logging.info("Reading Laser")
        M234Response = self.g.write("M234",resp_needed=True).split()
        logging.debug(M234Response)
        if int(M234Response[1]) > 27700:
            self.test.logFailure("Too far")
        else:
            self.test.logSuccess("Laser in Range")

        logging.info("Moving Z")
        G0Response = self.g.write("G0 Z40 F10000.0",resp_needed=True).split()
        logging.debug(G0Response)


        logging.info("Reading Laser")
        M234Response = self.g.write("M234",resp_needed=True).split()
        logging.debug(M234Response)
        if int(M234Response[1]) > 27700:
            self.test.logFailure("Too far")
        else:
            self.test.logSuccess("Laser in Range")

        logging.info("Reading Laser")
        M234Response = self.g.write("M234",resp_needed=True).split()
        logging.debug(M234Response)
        if int(M234Response[1]) > 27700:
            self.test.logFailure("Too far")
        else:
            self.test.logSuccess("Laser in Range")

        logging.info("Reading Laser")
        M234Response = self.g.write("M234",resp_needed=True).split()
        logging.debug(M234Response)
        if int(M234Response[1]) > 27700:
            self.test.logFailure("Too far")
        else:
            self.test.logSuccess("Laser in Range")

        logging.info("Reading Laser")
        M234Response = self.g.write("M234",resp_needed=True).split()
        logging.debug(M234Response)
        if int(M234Response[1]) > 27700:
            self.test.logFailure("Too far")
        else:
            self.test.logSuccess("Laser in Range")