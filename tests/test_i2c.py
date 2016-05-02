#! /usr/bin/env python

import unittest
import numpy as np
from mecode import G
from time import sleep
import re
from colorlog import ColoredFormatter
import logging

logging.basicConfig(level=logging.INFO,  # DEBUG, INFO, WARNING
                    format='%(asctime)s %(levelname)-8s %(message)s',
                    datefmt='%d-%m-%y %H:%M:%S',
                    filename='console.log',
                    filemode='w'
                    )

console = logging.StreamHandler()
console.setLevel(logging.ERROR)
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


class I2CTestCase(unittest.TestCase):

    def setUp(self):
        self.g = G(
            print_lines=False,
            aerotech_include=False,
            direct_write=True,
            direct_write_mode='serial',
            # printer_port="/dev/tty.usbmodem1421",
            printer_port="COM5",
        )

    def tearDown(self):
        # Make sure the previous test is fully finished before the next one
        # starts.
        self.g.write('M400', resp_needed=True)
        self.g.teardown()
        self.g = None

    def commandparser(self, command, p1='', p2='', p3=''):
        g = self.g
        resp = g.write(
            command + ' ' + p1 + ' ' + p2 + ' ' + p3, resp_needed=True)
        logging.debug(str(command + ' ' + p1 + ' ' + p2 + ' ' + p3))
        PacketSplit = re.findall('Packet', resp)
        TimeoutSplit = re.findall('Timeout', resp)
        if len(PacketSplit):
            logging.error(
                "Dropped " + str(len(PacketSplit)) + " Packets from address " + str(p1) + ". " + str(p1) + " is either not present or not communicating.")
            resp = 0
            self.assertEqual(PacketSplit, 1)
        if len(TimeoutSplit):
            logging.error(str(len(TimeoutSplit)) + " Timeouts from " + str(p1))
            resp = 0
            self.assertEqual(TimeoutSplit, 1)
        return resp

    def parseCartridgeStatus(self, cartridgeStatus, cartridgeNumber):
        logging.info("%s Info", cartridgeNumber)
        infoList = re.split('\W+', cartridgeStatus)
        logging.debug(infoList)
        logging.info("-------------------------------------")
        logging.info("Cartridge Serial Number: %s", infoList[2])
        logging.info("Cartridge Firmware Version: %s", infoList[18])
        logging.info("Cartridge Programmer Station: %s", infoList[5])
        logging.info("Cartridge Type: %s", infoList[8])
        logging.info("Cartridge Size: %s", infoList[11])
        logging.info("Cartridge Material: %s", infoList[14])

        if (int(infoList[18]) == 255):
            logging.error(
                "%s FLASH not loaded properly, or cartridge is out of date", cartridgeNumber)
            logging.info(' ')
            self.assertNotEqual(int(infoList[18]), 255,"Cartridge FLASH not loaded properly, or cartridge is out of date")

        if (int(infoList[8]) == 15) and (cartridgeNumber != "Cartridge Holder"):
            logging.error("%s EEPROM not loaded properly", cartridgeNumber)
            logging.info(' ')
            self.assertNotEqual(int(infoList[8]), 15, "Cartridge EEPROM not loaded properly")
        logging.info(' ')
    ###########################################################################
    #
    #                               Tests
    #
    ###########################################################################

    def test_i2c_alltargets(self):
        for i in range(0, 30):
            string = 'E'+str(i)
            self.commandparser('M244', 'C1', string)
            self.commandparser('M244', 'C0', string)

    def test_i2c_cart0(self):
        for i in range(0, 30):
            string = 'E'+str(i)
            self.commandparser('M244', 'C0', string)


    def test_i2c_cart2(self):
        for i in range(0, 30):
            string = 'E'+str(i)
            self.commandparser('M244', 'C1', string)

    def test_cart1(self):
        cart0status = self.commandparser('M245', 'C0')
        self.parseCartridgeStatus(cart0status, "Cartridge 0")

    def test_cart2(self):
        cart1status = self.commandparser('M245', 'C1')
        self.parseCartridgeStatus(cart1status, "Cartridge 1")

    def test_cartridgeholder(self):
        cartridgeHolderStatus = self.commandparser('M245', 'C2')
        self.parseCartridgeStatus(cartridgeHolderStatus, "Cartridge Holder")

if __name__ == '__main__':
    unittest.main()
