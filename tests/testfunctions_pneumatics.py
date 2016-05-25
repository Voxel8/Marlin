from mecode import G
import numpy as np
import logging
import re
from time import sleep


class Pneumatics_Test:

    """ Test module for the pneumatics system, e-regulator, and solenoid.

    This class is used to run tests on the pneumatics sled, and cartridge. 
    """

    def __init__(self, test, logging):
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

##########################################################################
# Utility Functions
##########################################################################

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

    def setPumpPressure(self, pressure):
        """ Sets the pressure of the pump

        Sets up the pressure of the Pneumatics Sled pump. Needs to be done prior
        to setting regulator pressure.

        Args:
            pressure (string): The desired pressure

        """
        self.g.write('M125 S{}'.format(pressure))

    def setRegulatorPressure(self, pressure):
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

        pumpPressure = self.g.write('M105', resp_needed=True)
        logging.debug(
            "Pump Pressure: {}".format(re.split('[:,\s]+', pumpPressure)))
        pumpPressure = re.split('[:,\s]+', pumpPressure)[14]
        return float(pumpPressure)

    def readRegulatorPressure(self):
        """ Reads the pressure of the regulator

        Reads the pressure of the regulator.

        Returns:
            int: Regulator pressure
        """
        regulatorPressure = self.g.write('M236', resp_needed=True)
        logging.debug(regulatorPressure.split())
        regulatorPressure = float(regulatorPressure.split()[0])
        return regulatorPressure

    def activateAndReadSolenoid(self, solenoid):
        """ Activates the solenoid and reads the status

        Activates the given solenoid using M380, and reads back
        the status.

        Args:
            solenoid (int): Desired Solenoid
        Returns:
            int: Solenoid Status

        """
        solenoidStatus = self.g.write('M380 T1 V', resp_needed=True).split()
        solenoidI2CStatus = self.commandparser('M253', 'C1').split()
        if self.testFailed is False:
            logging.debug(repr(solenoidStatus[3]))
            logging.debug(repr(solenoidI2CStatus[3]))
            return int(solenoidI2CStatus[3])
        else:
            return False

    def disableAndReadSolenoid(self, solenoid):
        """ Activates the solenoid and reads the status

        Deactivates the given solenoid using M381, and reads back
        the status.

        Args:
            solenoid (int): Desired Solenoid
        Returns:
            int: Solenoid Status
            
        """
        solenoidStatus = self.g.write('M381', resp_needed=True).split()
        solenoidI2CStatus = self.commandparser('M253', 'C1').split()
        if self.testFailed is False:
            logging.debug(repr(solenoidI2CStatus[3]))
            return int(solenoidI2CStatus[3])
        else:
            return False

    def activateAndReadSyringe(self):
        """ Activates the syringe and reads the status

        Activates the given syringe using M380, and reads back
        the status.

        Args:
            syringe (int): Desired Syringe
        Returns:
            int: Syringe Status

        """
        self.g.write('M42 P75 S255')
        sleep(1)
        syringeStatus = self.commandparser("M251").split()
        logging.debug(syringeStatus)
        if self.testFailed is False:
            return int(syringeStatus[3])
        else: 
            return False

    def disableAndReadSyringe(self):
        """ Activates the syringe and reads the status

        Deactivates the given syringe using M381, and reads back
        the status.

        Args:
            syringe (int): Desired Syringe
        Returns:
            int: Syringe Status
            
        """
        self.g.write('M42 P75 S0')
        sleep(2)
        syringeStatus = self.commandparser("M251").split()
        logging.debug(syringeStatus)
        if self.testFailed is False:
            return int(syringeStatus[3])
        else: 
            return False


##########################################################################
# Test Functions
##########################################################################

    def run_all_tests(self):
        """ Runs all of the enumerated tests, catch all for the file.

        Adding a test to this function is the equivalent of "installing" it
        in most unit testing frameworks. Tests can be individually called from
        the main file, but the main file likely just calls this one when there
        isn't a particular feature being tested.

        """
        self.test_for_leaks()
        self.test_set_pressure()

    def test_for_leaks(self):
        """ Tests tank pressure for stability

        Sets the tank pressure, and makes sure it reaches an acceptably close 
        level within a set period of time. Then checks for a set period of time 
        to make sure it mantains a close level to this value.

        Conditions:
            Pass: Reachs the set tank pressure in a reasonable amount of 
                time with a set accuracy, then makes sure it mantains that 
                pressure.
            Fail: After the set period of time, the tank never reaches the 
                appropriate pressure.
            Fail: After the tank reaches pressure, is unable to hold it for a 
                set length of time.

        """
        setPressure = 40
        getToPressureTime_s = 30
        pressureMeasurementTime_s = 40
        passingPressureRange = 1

        logging.info(
            "Testing for leaking Pressure Tank\n")
        self.setPumpPressure(setPressure)
        for i in range(getToPressureTime_s):
            pumpPressure = self.readPumpPressure()
            logging.debug("Pump Pressure: {} PSI".format(pumpPressure))
            if pumpPressure > setPressure - passingPressureRange:
                break
            sleep(1)
            if i is getToPressureTime_s - 1:
                self.test.logFailure(
                    "Tank Pressure did not reach accpetable pressure range")

        self.setPumpPressure(0)
        startingPumpPressure = self.readPumpPressure()
        for i in range(pressureMeasurementTime_s):

            pumpPressure = self.readPumpPressure()
            logging.debug("Pump Pressure: {} PSI".format(pumpPressure))
            if pumpPressure < startingPumpPressure - 1:
                self.testFailed = True
                self.test.logFailure("Tank Pressure too low, likely a leak")
                break
            sleep(1)
            if i is pressureMeasurementTime_s-1:
                self.test.logSuccess("Tank is mantaining pressure correctly")
            self.testFailed = False

    def test_set_pressure(self):
        """ Sets tank pressure, then tests the E-Reg at various 
            pressures and reads it back.

        Conditions:
            Pass: Reachs the enumerated pressures in a reasonable amount of 
                time and accuracy, after achieving the correct tank pressure
            Fail: After the set period of time, is not at the correct pressure 
                for any of the above.

        """
        PASSING_CRITERIA = .5

        logging.info(
            "Setting Pressure, checking accuracy\n")

        self.setPumpPressure(40)
        for i in range(10):
            pumpPressure = self.readPumpPressure()
            logging.debug("Pump Pressure: {} PSI".format(pumpPressure))
            if pumpPressure > 35:
                break
            sleep(1)
            if i is 9:
                self.test.logFailure("Tank Pressure too low")

        for i in range(0, 30, 5):
            if pumpPressure > i:
                self.setRegulatorPressure(i)
                sleep(1)
                regulatorPressure = self.readRegulatorPressure()
                logging.debug("Reg pressure: {} PSI".format(regulatorPressure))
                if i - regulatorPressure > PASSING_CRITERIA:
                    self.testFailed = True
                    self.test.logFailure(
                        "Pressure too low at {} PSI ({})".format(i, regulatorPressure))
                    break
                elif i - regulatorPressure < -PASSING_CRITERIA:
                    self.testFailed = True
                    self.test.logFailure(
                        "Pressure too high at {} PSI ({})".format(i, regulatorPressure))
                    break
                self.setRegulatorPressure(0)
        else:
            if self.testFailed is False:
                self.test.logSuccess("Pressure is appropriate")
            else:
                self.testFailed = False

    def test_solenoid(self):
        """ Opens and closes the solenoid, checking the status over I2C for both
            states.

        Conditions:
            Pass: I2C command reads correct solenoid condition
            Fail: I2C command incorrectly reads solenoid condition

        """
        logging.debug("Activating Solenoid")

        if self.activateAndReadSolenoid(1) is not 1 and self.testFailed is False:
                self.test.logFailure("Solenoid not activated")
                self.testFailed = True
        sleep(.25)
        if self.testFailed is False:
            logging.debug("Deactivating Solenoid")
            if self.disableAndReadSolenoid(1) is not 0 and self.testFailed is False:
                    self.test.logFailure("Solenoid not activated")
                    self.testFailed = True
        if self.testFailed is False:
            self.test.logSuccess("Solenoid operating correctly")
        else:
            self.testFailed = False

    def test_syringe(self):
        """ Drops the syringe and retracts it, checking the status over I2C for both
            states.

        Conditions:
            Pass: I2C command reads correct syringe condition
            Fail: I2C command incorrectly reads syringe condition

        """
        logging.debug("Activating Syringe")
        if (self.activateAndReadSyringe()) is not 1 and self.testFailed is False:
            self.test.logFailure("Syringe not activated")
            self.testFailed = True
        if self.testFailed is False:
            logging.debug("Retracting Syringe")
            if (self.disableAndReadSyringe()) is not 0 and self.testFailed is False:
               self.test.logFailure("Syringe retraction not functioning")
               self.testFailed = True
        if self.testFailed is False:
            self.test.logSuccess("Syringe operating correctly")
        else:
            self.testFailed = False

