#! /usr/bin/env python

import unittest
import numpy as np
from mecode import G
from time import sleep
import re

class I2CTestCase(unittest.TestCase):

    def setUp(self):
        self.g = G(
            print_lines=False,
            aerotech_include=False,
            direct_write=True,
            direct_write_mode='serial',
            #printer_port="/dev/tty.usbmodem1421",
            printer_port="COM5",
        )

    def tearDown(self):
        # Make sure the previous test is fully finished before the next one
        # starts.
        self.g.write('M400', resp_needed=True)
        self.g.teardown()
        self.g = None

    def commandparser(self,command, p1 = '', p2 = '', p3=''):
    	g = self.g
    	resp = g.write(command + ' ' + p1+ ' ' + p2 + ' ' + p3, resp_needed=True);
    	print(str(command + ' ' + p1+ ' ' + p2 + ' ' + p3))
    	testsplit = re.findall('Packet',resp)
    	if len(testsplit):
    		print("Dropped " + str(len(testsplit)) + " Packets from address " + str(p1))
    	testsplit = re.findall('Timeout',resp)
    	if len(testsplit):
    		print(str(len(testsplit)) + " Timeouts from " + str(p1))
    ###########################################################################
    #
    #                               Tests
    #
    ###########################################################################

    def test_i2c(self):
      	self.commandparser('M245','C0')
      	self.commandparser('M245','C1')
      	for i in range(0,30):
      		string = 'E'+str(i)
      		self.commandparser('M244','C0',string)
      		self.commandparser('M244','C1',string)
if __name__ == '__main__':
    unittest.main()