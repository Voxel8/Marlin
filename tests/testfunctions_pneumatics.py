from mecode import G
import numpy as np
import logging
import re
from time import sleep


class Pneumatics_Test:
    """ Test module for the pneumatics system, e-regulator, and solenoid.

    This class is used to run tests on the pneumatics sled, and cartridge. 
    """
    
    def __init__(self,test,logging):
        """Initialize the Pneumatics test, with the supplied overall test and logger.

        Args:
            test (testRunner): The testrunner used to call the Pneumatics Test, 
                allowing the use of test failing / passing / warnings.
            logging (logger): Allows whatever preconfigured logging module used
                in the main program to be accessed in this file..
        
        """
        self.g = test.g
        self.test = test
        logging.debug("Initialized Ereg Test")
        self.testFailed = False

################################################################################
## Utility Functions
################################################################################


    def setPumpPressure(self,pressure):
        """ Sets the pressure of the pump

        Sets up the pressure of the Pneumatics Sled pump. Needs to be done prior
        to setting regulator pressure.

        Args:
            pressure (string): The desired pressure
        
        """
        self.g.write('M125 S{}'.format(pressure))

    def setRegulatorPressure(self,pressure):
        """ Sets the pressure of the regulator

        Sets the pressure of the regulator.

        Args:
            pressure (string): The desired pressure

        """
        self.g.write('M236 S{}'.format(pressure))   

    def readPumpPressure(self):
        """ Reads the pressure of the pump

        Reads the pressure of the pump. Splits the output from M105 to get the 
        relevant number

        Returns:
            float: Pump pressure
        """

        pumpPressure = self.g.write('M105',resp_needed = True)
        logging.debug("Pump Pressure: {}".format(re.split('[:,\s]+', pumpPressure)))
        pumpPressure = re.split('[:,\s]+', pumpPressure)[14]
        return float(pumpPressure)

    def readRegulatorPressure(self):
        """ Reads the pressure of the regulator

        Reads the pressure of the regulator.

        Returns:
            int: Regulator pressure
        """
        regulatorPressure = self.g.write('M236',resp_needed = True)
        logging.debug(regulatorPressure.split())
        regulatorPressure = float(regulatorPressure.split()[0])
        return regulatorPressure


################################################################################
## Test Functions
################################################################################
    def run_all_tests(self):
        """ Runs all of the enumerated tests, catch all for the file.
        
        Adding a test to this function is the equivalent of "installing" it
        in most unit testing frameworks. Tests can be individually called from
        the main file, but the main file likely just calls this one when there
        isn't a particular feature being tested.

        """
        self.test_set_pressure()

    def test_set_pressure(self):
        """ Sets tank pressure, then tests the E-Reg at various 
            pressures and reads it back.

        Conditions:
            Pass: Reachs the enumerated pressures in a reasonable amount of 
                time and accuracy, after achieving the correct tank pressure
            Fail: After the set period of time, is not at the correct pressure 
                for any of the above.

        """
        logging.info(
            "Setting Pressure, checking accuracy\n")

        self.setPumpPressure(40)
        for i in range (10):
            pumpPressure = self.readPumpPressure()
            logging.debug("Pump Pressure: {} PSI".format(pumpPressure))
            if pumpPressure > 35:
                break
            sleep(1)
            if i is 9:
                self.test.logFailure("Tank Pressure too low")

        pressure_bound = .5
        for i in range(0,30,5):
            if pumpPressure > i:
                self.setRegulatorPressure(i)
                sleep(1)
                regulatorPressure = self.readRegulatorPressure()
                logging.debug("Reg pressure: {} PSI".format(regulatorPressure))
                if i - regulatorPressure > pressure_bound:
                    self.testFailed = True
                    self.test.logFailure("Pressure too low at {} PSI ({})".format(i,regulatorPressure))
                    break
                elif i - regulatorPressure < -pressure_bound:
                    self.testFailed = True
                    self.test.logFailure("Pressure too high at {} PSI ({})".format(i,regulatorPressure))
                    break
                self.setRegulatorPressure(0)
        else:
            if self.testFailed is False:
                    self.test.logSuccess("Pressure is appropriate")
    