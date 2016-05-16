from mecode import G
import numpy as np
import logging
import re
from time import sleep


class Ereg_Test:
    
    def __init__(self,test,logging):
        self.g = test.g
        self.test = test
        logging.debug("Initialized Ereg Test")

    def run_all_tests(self):
        pass