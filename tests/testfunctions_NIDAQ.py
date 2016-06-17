from mecode import G
import numpy as np
import logging
import re
from time import sleep
import sys
sys.path.append("./Diagnostics_Suite")
from TestUtilities import TestRunner, ResponseData, cd

import os
import time
import sys
import serial

from datetime import date
from mecode import G
from nidaqmx import AnalogInputTask
from threading import Thread
from testfunctions_pneumatics import Pneumatics_Test


path = r"\\V8NETDRIVE\home\Regulator Testing Data" # Path where data folder is located
current_ramp_cycle = None                          # Current pressure ramp cycle, used for logging purposes


class Pneumatics_Test_NIDAQ(object):

    """ Test module for the pneumatics system, e-regulator, and solenoid.

    This class is used to run tests on the pneumatics sled, and cartridge. 
    """

    def __init__(self, test, logging, regulator):
        """Initialize the NIDAQ extended Pneumatics test, with the supplied overall test and logger.

        Args:
            test (testRunner): The testrunner used to call the Pneumatics Test, 
                allowing the use of test failing / passing / warnings.
            logging (logger): Allows whatever preconfigured logging module used
                in the main program to be accessed in this file..

        """
        self.g = test.g
        self.test = test
        logging.debug("Initialized NIDAQ Extend Pneumatics Test")
        self.pneumatics = Pneumatics_Test(test, logging)

        # Basic defines for running the test, used further on
        self.pressure_ramp_cycles = 1            # Total number cycles the test is run
        self.sample_rate = 1000                  # Sampling rate (Hz)
        self.solenoid_toggle_cycles = 1          # Number of solenoid_toggle_cycles to toggle solenoid at each pressure
        self.wait_time_s = 1                     # Wait for regulator to reach pressure in between solenoid toggles
        self.checkpoint = 5                      # Save full data every _ solenoid solenoid_toggle_cycles, if 0 don't save
        self.ID = str(regulator)                 # Name of the file saved in the directory by the logger
        
        # Miscellanous setup
        self.outputdirectory = None              # Allows us ot set the output directory later with the logger
        self.task = AnalogInputTask()            # Used to read from the DAQ
        self.task.create_voltage_channel('Dev1/ai0', terminal='diff',
                                    min_val=-5.0, max_val=5.0)
        self.task.configure_timing_sample_clock(rate=self.sample_rate)
        with cd(path):
            self.daq = DAQ(self)                 # Sets the directory that the logger logs in

    def record_solenoid_pressure(self, pressure):
        # Set regulator pressure
        self.pneumatics.setRegulatorPressure(pressure)
        time.sleep(3)
        for cycle in range(0, self.solenoid_toggle_cycles):
            self.daq.record(int(self.sample_rate*self.wait_time_s),
                       cycle, pressure)                # Open recording thread
            self.open_solenoid()   
            time.sleep(self.wait_time_s)
            self.close_solenoid()
            time.sleep(1)

    def test_Diagnostics_trial_pump(self):
        t = ResponseData()

        # Test Failure Criteria
        pump_to_pressure_time = 60
        pump_pressure = 40
        pump_pressure_acceptable_bound = 2
        pressure_tank_start = 0             # Beginning pressure while testing for tank pressure
        pressure_tank_end = 30              # Highest pressure reached while testing on tank pressure
        pressure_interval = 5               # Interval in which the pressure increases between bounds

        global current_ramp_cycle
        with cd(path):
            g = self.g
            task = AnalogInputTask()

            self.close_house_air()

            for i in range(0, self.pressure_ramp_cycles):

                # Ensure solenoid is closed at start
                self.close_solenoid()
                
                # Set tank pressure to pump pressure
                self.pneumatics.setPumpPressure(pump_pressure)
                
                # Wait for pump to reach pressure
                for i in range(pump_to_pressure_time):
                    p = self.pneumatics.readPumpPressure()
                    if p > pump_pressure - pump_pressure_acceptable_bound:
                        break
                    if i is pump_to_pressure_time - 1:
                        t.fail("Didn't reach pump pressure")
                        return t
                    sleep(1)
                    logging.debug("Pump Pressure: {}".format(p))


                logging.info("Testing Output Pressure (Source: Tank)")
                current_ramp_cycle = i
                # Interval added / subtracted to help make range function more
                # intuitive
                for p in range(pressure_tank_start, pressure_tank_end + pressure_interval, pressure_interval):
                    logging.debug(p)
                    self.record_solenoid_pressure(p)
                    if self.daq.u.failed:
                        self.close_house_air()
                        self.pneumatics.setRegulatorPressure(0)
                        self.pneumatics.setPumpPressure(0)
                        if (p >= 10 or p <= 25):
                            t = self.daq.u
                            return t

                for p in range(pressure_tank_end, pressure_tank_start - pressure_interval, -pressure_interval):
                    logging.debug(p)
                    self.record_solenoid_pressure(p)
                    if self.daq.u.failed:
                        self.close_house_air()
                        self.pneumatics.setRegulatorPressure(0)
                        self.pneumatics.setPumpPressure(0)
                        print(self.daq.u.data.split())
                        t = self.daq.u
                        return t                        

            self.close_house_air()
            
            self.daq.display_test_results()
            t.success("Pressure test run successfully")
            return t

    def test_Diagnostics_trial_house(self):
        t = ResponseData()

        # Test Failure Criteria
        pressure_house_start = 30           # Beginning of house air pressure test
        pressure_house_end = 80             # Highest pressure reached while testing house air
        pressure_interval = 5               # Interval in which the pressure increases between bounds

        global current_ramp_cycle
        with cd(path):
            g = self.g
            task = AnalogInputTask()

            self.close_house_air()

            for i in range(0, self.pressure_ramp_cycles):
                # Ensure solenoid is closed at start
                # Ensure Solenoid is closed
                self.close_solenoid()
                # Set tank pressure to 40psi
                self.pneumatics.setPumpPressure(40)

            logging.info("Testing Output Pressure (Source: House Air)")

            self.open_house_air()

            for i in range(0, self.pressure_ramp_cycles):
                # Ensure solenoid is closed at start
                # Ensure Solenoid is closed
                self.close_solenoid()
                # Set tank pressure to 40psi
                self.pneumatics.setPumpPressure(40)

                current_ramp_cycle = i
                # Interval added / subtracted to help make range function more
                # intuitive
                for p in range(pressure_house_start, pressure_house_end + pressure_interval, pressure_interval):
                    logging.debug(p)
                    self.record_solenoid_pressure(p)
                    if self.daq.u.failed:
                        self.close_house_air()
                        self.pneumatics.setRegulatorPressure(0)
                        self.pneumatics.setPumpPressure(0)

                for p in range(pressure_house_end, pressure_house_start - pressure_interval, -pressure_interval):
                    logging.debug(p)
                    self.record_solenoid_pressure(p)
                    if self.daq.u.failed:
                        self.close_house_air()
                        self.pneumatics.setRegulatorPressure(0)
                        self.pneumatics.setPumpPressure(0)
            
            self.close_house_air()
            
            self.daq.display_test_results()
            t.success("Pressure test run successfully")
            return t


    def open_solenoid(self):
        self.g.write('M42 P8 S255')          # Open solenoid (Fan 0)

    def close_solenoid(self):
       self.g.write('M42 P8 S0')            # Close solenoid (Fan 0)

    def open_house_air(self):
        self.g.write('M42 P6 S255')          # Open solenoid (Fan 1)

    def close_house_air(self):
        self.g.write('M42 P6 S0')            # Close solenoid (Fan 1)
##########################################################################


class DAQ(object):

    """Interface for the NI DAQ"""

    def __init__(self, test):
        """Initializes the DAQ with a task, and a logger"""
        self.task1 = test.task
        self.data = {}
        self.log = Logger(test)
        self.g = test.g
        self.wait_time_s = test.wait_time_s
        self.sample_rate = test.sample_rate
        self.checkpoint = test.checkpoint
        self.u = ResponseData()
        self.test = test
        
    def record(self, samples, cycle, pressure):
        """Starts a thread that launches a record_worker"""
        self._rec_thread = Thread(target=self.record_worker,
                                  args=(samples, cycle, pressure))
        self._rec_thread.start()

    def record_worker(self, samples, cycle, pressure):

        # Test Warning Criteria
        error_actual_warning_bound = .5
        error_reported_warning_bound = 1
        response_time_warning_bound = .25

        # Test Failing Criteria
        error_actual_bound = 1
        error_reported_bound = 2
        response_time_bound = .35 
        pressure_failing_low = 5
        pressure_failing_high = 25
        response_time_tolerance = 0.95                    # The percentage of the set value that, when reached, will be marked as reaching the value.     


        global current_ramp_cycle
        """Records the number of samples losted, logs the data"""
        # Record data
        self.task1.start()
        self.data[cycle] = self.task1.read(samples)
        self.task1.stop()

        resp = (self.g.write('M236', resp_needed=True))
        pressure_reported = (resp.split()[0])
        logging.debug("pressure reported {} ".format(pressure_reported))
        # Save times required to reach tolerance point
        timestamp = self.wait_time_s
        last_timestamp = self.wait_time_s
        final_value = float(np.average(self.data[cycle][-10:-5]))
        target = response_time_tolerance * final_value 
        error_actual = float(final_value*20) - float(pressure)
        error_reported = float(pressure_reported) - float(final_value*20)
        for value in reversed(self.data[cycle]):
            if value < target:
                self.log.log_and_print(cycle, pressure, float(
                    final_value)*20.0, error_actual, pressure_reported, error_reported, last_timestamp)
                if abs(error_actual) > error_actual_bound  or  abs(error_reported) > error_reported_bound:
                    if pressure >= pressure_failing_low and pressure <= pressure_failing_high:
                        self.u.fail("Error too high, test failed: Actual: {:2f}, Reported {:2f}, Pressure {}".format(error_actual, error_reported, pressure))
                    elif abs(error_actual) > error_actual_warning_bound  or  abs(error_reported) > error_reported_warning_bound:
                        self.test.test.logWarning("Error may be outside of acceptable range: Actual: {:2f} PSI Reported: {:2f} PSI, Pressure {}".format(error_actual, error_reported, pressure))
                elif last_timestamp > response_time_bound and pressure > 0:
                    self.u.fail("Response time too high, test failed: Time: {}, Pressure: {}".format(last_timestamp, pressure))
                elif last_timestamp > response_time_warning_bound and pressure > 0:
                    self.test.test.logWarning("Response time might be outside of acceptable range: Time {:2f}, Pressure: {}".format(last_timestamp, pressure))
                break
            last_timestamp = timestamp
            timestamp -= (1.0 / self.sample_rate)
            if timestamp is (1.0 / self.sample_rate):
                self.log.log_and_print(cycle, pressure, float(
                    final_value)*20.0, error_actual, pressure_reported, error_reported, self.wait_time_s)
                if pressure is not 0:
                    u.fail("Response time too long, test failed")
        # Periodically save full data for a cycle
        if cycle % self.checkpoint == 0:
            self.log.log_checkpoint(
                self.data, cycle, current_ramp_cycle, pressure)

    def display_test_results(self):
        """Shows the final display from the stat tracker in the logger"""
        self.log.display_test_results()
###############################################################################


class Logger(object):

    def __init__(self, test):

        self.sample_rate = test.sample_rate
        self.ID = test.ID
        """Creates the output directory, Response_Data.csv, populates the header, and makes a statTrackerCollection"""
        i = 1
        today = '{0}{1:02}{2:02}'.format(date.today().year-2000,
                                         date.today().month,
                                         date.today().day)
        self.outdir = 'data/ereg_{0}_{1:02}_{2}'.format(today, i, self.ID)
        for _ in os.listdir('data'):
            if '_'.join(self.outdir[5:].split('_')[0:3]) in _:
                i += 1
                self.outdir = 'data/ereg_{0}_{1:02}_{2}'.format(today, i, self.ID)
        self.outdir = 'data/ereg_{0}_{1:02}_{2}'.format(today, i, self.ID)
        os.makedirs(self.outdir)

        self.response_time = os.path.join(self.outdir, 'Response_Data.csv')
        with open(self.response_time, 'a') as f:
            f.write(self.ID + '\n\n')
            f.write(
                'Ramp Cycle, Cycle, Set Pressure, Actual Pressure, Actual Error, Reported Pressure, Reported Error, Response Time ,Value less than Target\n')
        ##  f.write('{7}, {0},   {1},          {2:.2f},         {3:.2f},      {4},               {5:.2f},        {6:.2f} ')

        self.statTracker = PSI_StatTracker_Collection(self.outdir)

    def log_and_print(self, cycle, pressure, pressure_actual, error_actual, pressure_reported, error_reported, last_timestamp):
        """"Logs and prints the test data in the correct format"""
        with open(self.response_time, 'a') as f:
            f.write('{7}, {0}, {1}, {2:.2f}, {3:.2f}, {4}, {5:.2f}, {6:.2f},\n'.format(
                cycle, pressure, pressure_actual, error_actual, pressure_reported, error_reported, last_timestamp, current_ramp_cycle))

        logging.debug('Ramp Cycle: {7}, Cycle: {0}, Set Pressure {1}, Actual PSI {2:.2f}, Actual Err {3:.2f}, Reported PSI {4}, Reported Err {5:.2f}, Response Time {6:.2f},\n'.format(
            cycle, pressure, pressure_actual, error_actual, pressure_reported, error_reported, last_timestamp, current_ramp_cycle))

        tracker = self.statTracker.getStatTracker(pressure)
        tracker.pressure_err_update(error_actual)
        tracker.response_time_update(last_timestamp)

    def log_checkpoint(self, data, cycle, ramp_cycle, psi):
        """Logs the data for a cycle, called after a certain interval to not spam"""
        outfile = os.path.join(
            self.outdir, '{0}_{1}_{2}.csv'.format(cycle, ramp_cycle, psi))
        with open(outfile, 'w') as f:
            timestamp = 0.0
            for value in data[cycle]:
                timestamp += (1.0 / self.sample_rate)
                f.write(
                    '{0:.3f}, {1:.3f}\n'.format(float(timestamp), float(value)*20.0))

    def display_test_results(self):
        """Displays the test data kept by the statTracker"""
        self.statTracker.finalDisplay()
###############################################################################


class PSI_StatTracker(object):

    def __init__(self, setpressure):
        """Initializes all stats to be tracked, sets pressure label as the passed pressure"""
        ############## Output #############
        # Running Test X on Y
        # Set Pressure: X
        # Input Pressure: X (from M105)
        # Average Response Time: X @ Y         Response time = last_timestamp
        # Max Response Time:     X @ Y PSI     Largest of last_timestamp
        # Last Response Time:    X @ Y PSI     last_timestamp

        # Average Press Err:     X @ Y PSI     error(set, pressure)
        # Max Press Error:       X @ Y PSI     largest of above
        # Average Reported Err:  X @ Y PSI     error(pressure, pressure reported)
        # Test: Pass / Fail                    check parameters on the above
        # Hysteresis                           1 - 30, 2 - 29, etc

        self.set_pressure = setpressure

        self.response_time_max = 0
        self.response_time_last = 0
        self.response_time_avg = 0
        self.response_time_measurements = 0

        self.pressure_err_max = 0
        self.pressure_err_last = 0
        self.pressure_err_avg = 0
        self.pressure_err_measurements = 0

        self.reported_pressure_err_max = 0
        self.reported_pressure_err_last = 0
        logging.debug("Stat Tracker {0}".format(self.set_pressure))

    def pressure_err_update(self, new_pressure):
        """Updates the pressure average, last pressure, and decides if it's the highest seen pressure"""
        self.pressure_err_measurements = self.pressure_err_measurements + 1
        self.pressure_err_avg = self.compute_average(
            self.pressure_err_avg, self.pressure_err_measurements, new_pressure)
        self.pressure_err_last = new_pressure
        if abs(new_pressure) > abs(self.pressure_err_max):
            self.pressure_err_max = new_pressure

    def response_time_update(self, new_response_time):
        """Updates the response time average, last response time, and decides if it's the highest seen response time"""
        self.response_time_measurements = self.response_time_measurements + 1
        self.response_time_avg = self.compute_average(
            self.response_time_avg, self.response_time_measurements, new_response_time)
        self.response_time_last = new_response_time
        if new_response_time > self.response_time_max:
            self.response_time_max = new_response_time

    def compute_average(self, average, measurements, new_value):
        """Finds the average using finite analysis"""
        return ((measurements-1) * average + new_value) / measurements

    def display_data(self):
        """Prints the data kept in this stat tracker"""
        logging.debug("Set PSI: {0:.2f} PSI".format(self.set_pressure))
        logging.debug("-----Response Time-----")
        logging.debug("Max Response Time: {0:.2f} seconds".format(
            self.response_time_max))
        logging.debug("Last Response Time: {0:.2f} seconds".format(
            self.response_time_last))
        logging.debug("Average Response Time: {0:.2f} seconds".format(
            self.response_time_avg))
        logging.debug("--------Pressure-------")
        logging.debug("Max Pressure Error: {0:.2f} PSI".format(self.pressure_err_max))
        logging.debug("Last Pressure Error: {0:.2f} PSI".format(
            self.pressure_err_last))
        logging.debug("Average Pressure Error: {0:.2f} PSI".format(
            self.pressure_err_avg))
###############################################################################


class PSI_StatTracker_Collection(object):

    def __init__(self, directory):
        """Creates the list of stat trackers, initializes final statistics"""
        self.statTrackerList = []
        self.pressure_err_max = 0
        self.pressure_err_max_set_pressure = 0
        self.response_time_max = 0
        self.response_time_max_set_pressure = 0
        self.directory = directory

    def getStatTracker(self, pressure):
        """Attempts to return the stat tracker for the appropriate pressure, and creates a new
        stat tracker if one doesn't exist"""
        for i in range(0, len(self.statTrackerList)):
            if self.statTrackerList[i].set_pressure == pressure:
                return self.statTrackerList[i]
        self.statTrackerList.append(PSI_StatTracker(pressure))
        return self.statTrackerList[len(self.statTrackerList)-1]

    def finalDisplay(self):
        outfile = os.path.join(self.directory, 'final_display.txt')
        """Gathers the data for the final display, and prints it"""
        with open(outfile, 'w') as f:
            logging.debug("\nTest Results")
            logging.debug("-------------------")
            l = self.statTrackerList
            for i in range(0, len(l)):
                if l[i].response_time_max > self.response_time_max:
                    self.response_time_max = l[i].response_time_max
                    self.response_time_max_set_pressure = l[i].set_pressure
            logging.debug("Max response time: {0} seconds at {1} PSI".format(
                self.response_time_max, self.response_time_max_set_pressure))
            f.write("Max response time: {0} seconds at {1} PSI\n".format(
                self.response_time_max, self.response_time_max_set_pressure))
            for i in range(0, len(l)):
                if abs(l[i].pressure_err_max) > abs(self.pressure_err_max):
                    self.pressure_err_max = abs(l[i].pressure_err_max)
                    self.pressure_err_max_set_pressure = l[i].set_pressure
            logging.debug("Max pressure error: {0:.2f} PSI at {1} PSI".format(
                self.pressure_err_max, self.pressure_err_max_set_pressure))
            f.write("Max pressure error: {0:.2f} PSI at {1} PSI\n".format(
                self.pressure_err_max, self.pressure_err_max_set_pressure))
            for i in range(0, len(l)):
                logging.debug("Pressure: {0} psi. Avg Error: {1:.2f} psi. Avg Response Time: {2} seconds. Last Error: {3:.2f} psi. Last Response Time {4} seconds".format(
                    l[i].set_pressure, l[i].pressure_err_avg, l[i].response_time_avg, l[i].pressure_err_last, l[i].response_time_last))
                f.write("Pressure: {0} psi. Avg Error: {1:.2f} psi. Avg Response Time: {2} seconds. Last Error: {3:.2f} psi. Last Response Time {4} seconds\n".format(
                    l[i].set_pressure, l[i].pressure_err_avg, l[i].response_time_avg, l[i].pressure_err_last, l[i].response_time_last))
