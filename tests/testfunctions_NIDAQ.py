from mecode import G
import numpy as np
import logging
import re
from time import sleep
import sys
sys.path.append("./Diagnostics_Suite")
from TestUtilities import TestRunner, ResponseData

import os
import time
import sys
import serial

from datetime import date
from mecode import G
from nidaqmx import AnalogInputTask
from threading import Thread
from testfunctions_pneumatics import Pneumatics_Test



class Pneumatics_Test_NIDAQ:

    """ Test module for the pneumatics system, e-regulator, and solenoid.

    This class is used to run tests on the pneumatics sled, and cartridge. 
    """

    def __init__(self, test, logging):
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
        self.pneumatics = Pneumatics_Test(test,logging)

        self.pressure_start = 0                  # Pressure to toggle regulator
        self.pressure_end = 40
        self.pressure_interval = 5
        self.pressure_ramp_cycles = 1            # Total number of solenoid_toggle_cycles
        self.sample_rate = 1000                  # Sampling rate (Hz)
        
        self.tolerance = 0.95                    # Response-time tolerance
        # Number of solenoid_toggle_cycles to toggle solenoid
        self.solenoid_toggle_cycles = 1
        #   at each pressure
        self.wait_time_s = 1                     # Wait for regulator to reach pressure
        # Save full data every _ solenoid solenoid_toggle_cycles, if 0 don't save
        self.checkpoint = 5
        self.ID = 'Default'
        
        self.outputdirectory = None
        self.current_ramp_cycle = None


        self.task = AnalogInputTask()
        task.create_voltage_channel('Dev1/ai0', terminal='diff',
                            min_val=-5.0, max_val=5.0)
        task.configure_timing_sample_clock(rate=sample_rate)
        self.daq  = DAQ(self.task)


    def test_solenoid_pressure(pressure):
        self.setRegulatorPressure(pressure)       # Set regulator pressure
        time.sleep(3)
        for cycle in range(0, solenoid_toggle_cycles):
            self.daq.record(int(sample_rate*wait_time_s),
                       cycle, p)                # Open recording thread
    
            self.pneumatics.activateAndReadSolenoid()         # Open solenoid
            time.sleep(wait_time_s)
            self.pneumatics.disableAndReadSolenoid()          # close Solenoid
            time.sleep(1)


    def test_Diagnostics_trial:
        path = r"\\V8NETDRIVE\home\Regulator Testing Data"
    
        with cd(path):
            g = G(print_lines=False,
                aerotech_include=False,
                direct_write=True,
                direct_write_mode='serial',
                printer_port='COM18')
            
            task = AnalogInputTask()

        
        
            for i in range(0, pressure_ramp_cycles):
                # Ensure solenoid is closed at start
                g.write('M42 P8 S0')
                g.write('M125 S40')                     # Set tank pressure to 40psi
        
                current_ramp_cycle = i
                # Interval added / subtracted to help make range function more intuitive
                for p in range(self.pressure_start, self.pressure_end + self.pressure_interval, self.pressure_interval):
                    print(p)
                    self.solenoid_pressure_test(p)
            
                for p in range(self.pressure_end, self.pressure_start - self.pressure_interval, -self.pressure_interval):
                    print(p)
                    self.solenoid_pressure_test(p)
            
            self.g.teardown()
            
            self.daq.display_test_results()
    
###########################################################################################
class DAQ(object):

    """Interface for the NI DAQ"""

    def __init__(self, task1):
        """Initializes the DAQ with a task, and a logger"""
        self.task1 = task1
        self.data = {}
        self.log = Logger()

    def record(self, samples, cycle, pressure):
        """Starts a thread that launches a record_worker"""
        self._rec_thread = Thread(target=self.record_worker,
                                  args=(samples, cycle, pressure))
        self._rec_thread.start()

    def record_worker(self, samples, cycle, pressure):
        """Records the number of samples losted, logs the data"""
        # Record data
        self.task1.start()
        self.data[cycle] = self.task1.read(samples)
        self.task1.stop()

        resp = (g.write('M236', resp_needed=True))
        pressure_reported = (resp.split()[0])
        print ("pressure reported {} ".format(pressure_reported))
        # Save times required to reach tolerance point
        target = tolerance * (pressure / 20.0)
        timestamp = wait_time_s
        last_timestamp = wait_time_s
        final_value = float(self.data[cycle][-10])
        error_actual = float(final_value*20) - float(pressure)
        error_reported = float(pressure_reported) - float(final_value*20)
        for value in reversed(self.data[cycle]):
            if value < target:
                self.log.log_and_print(cycle, pressure, float(
                    final_value)*20.0, error_actual, pressure_reported, error_reported, last_timestamp)
                break
            last_timestamp = timestamp
            timestamp -= (1.0 / sample_rate)
            if timestamp is (1.0 / sample_rate):
                self.log.log_and_print(cycle, pressure, float(
                    final_value)*20.0, error_actual, pressure_reported, error_reported, wait_time_s)

        # Periodically save full data for a cycle
        if cycle % checkpoint == 0:
            self.log.log_checkpoint(
                self.data, cycle, current_ramp_cycle, pressure)

    def display_test_results(self):
        """Shows the final display from the stat tracker in the logger"""
        self.log.display_test_results()


###############################################################################

class Logger(object):

    def __init__(self):
        """Creates the output directory, Response_Data.csv, populates the header, and makes a statTrackerCollection"""
        i = 1
        today = '{0}{1:02}{2:02}'.format(date.today().year-2000,
                                         date.today().month,
                                         date.today().day)
        self.outdir = 'data/ereg_{0}_{1:02}_{2}'.format(today, i, ID)
        for _ in os.listdir('data'):
            if '_'.join(self.outdir[5:].split('_')[0:3]) in _:
                i += 1
                self.outdir = 'data/ereg_{0}_{1:02}_{2}'.format(today, i, ID)
        self.outdir = 'data/ereg_{0}_{1:02}_{2}'.format(today, i, ID)
        os.makedirs(self.outdir)

        self.response_time = os.path.join(self.outdir, 'Response_Data.csv')
        with open(self.response_time, 'a') as f:
            f.write(ID + '\n\n')
            f.write(
                'Ramp Cycle, Cycle, Set Pressure, Actual Pressure, Actual Error, Reported Pressure, Reported Error, Response Time ,Value less than Target\n')
        ##  f.write('{7}, {0},   {1},          {2:.2f},         {3:.2f},      {4},               {5:.2f},        {6:.2f} ')

        self.statTracker = PSI_StatTracker_Collection(self.outdir)

    def log_and_print(self, cycle, pressure, pressure_actual, error_actual, pressure_reported, error_reported, last_timestamp):
        """"Logs and prints the test data in the correct format"""       
        with open(self.response_time, 'a') as f:
            f.write('{7}, {0}, {1}, {2:.2f}, {3:.2f}, {4}, {5:.2f}, {6:.2f},\n'.format(
                cycle, pressure, pressure_actual, error_actual, pressure_reported, error_reported, last_timestamp, current_ramp_cycle))

        print('Ramp Cycle: {7}, Cycle: {0}, Set Pressure {1}, Actual PSI {2:.2f}, Actual Err {3:.2f}, Reported PSI {4}, Reported Err {5:.2f}, Response Time {6:.2f},\n'.format(
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
                timestamp += (1.0 / sample_rate)
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
        print("Stat Tracker {0}".format(self.set_pressure))

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
        print("Set PSI: {0:.2f} PSI".format(self.set_pressure))
        print("-----Response Time-----")
        print("Max Response Time: {0:.2f} seconds".format(
            self.response_time_max))
        print("Last Response Time: {0:.2f} seconds".format(
            self.response_time_last))
        print("Average Response Time: {0:.2f} seconds".format(
            self.response_time_avg))
        print("--------Pressure-------")
        print("Max Pressure Error: {0:.2f} PSI".format(self.pressure_err_max))
        print("Last Pressure Error: {0:.2f} PSI".format(
            self.pressure_err_last))
        print("Average Pressure Error: {0:.2f} PSI".format(
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
            print("\nTest Results")
            print("-------------------")
            l = self.statTrackerList
            for i in range(0, len(l)):
                if l[i].response_time_max > self.response_time_max:
                    self.response_time_max = l[i].response_time_max
                    self.response_time_max_set_pressure = l[i].set_pressure
            print("Max response time: {0} seconds at {1} PSI".format(
                self.response_time_max, self.response_time_max_set_pressure))
            f.write("Max response time: {0} seconds at {1} PSI\n".format(
                self.response_time_max, self.response_time_max_set_pressure))
            for i in range(0, len(l)):
                if abs(l[i].pressure_err_max) > abs(self.pressure_err_max):
                    self.pressure_err_max = abs(l[i].pressure_err_max)
                    self.pressure_err_max_set_pressure = l[i].set_pressure
            print("Max pressure error: {0:.2f} PSI at {1} PSI".format(
                self.pressure_err_max, self.pressure_err_max_set_pressure))
            f.write("Max pressure error: {0:.2f} PSI at {1} PSI\n".format(
                self.pressure_err_max, self.pressure_err_max_set_pressure))
            for i in range(0, len(l)):
                print("Pressure: {0} psi. Avg Error: {1:.2f} psi. Avg Response Time: {2} seconds. Last Error: {3:.2f} psi. Last Response Time {4} seconds".format(
                    l[i].set_pressure, l[i].pressure_err_avg, l[i].response_time_avg, l[i].pressure_err_last, l[i].response_time_last))
                f.write("Pressure: {0} psi. Avg Error: {1:.2f} psi. Avg Response Time: {2} seconds. Last Error: {3:.2f} psi. Last Response Time {4} seconds\n".format(
                    l[i].set_pressure, l[i].pressure_err_avg, l[i].response_time_avg, l[i].pressure_err_last, l[i].response_time_last))

###############################################################################


# # Parse all active COM ports
# comports = serial_ports()
# #print('Avalable COM ports: ' + str(comports) + '. Using ' + comports[0] + '...')

def test_Diagnostics_trial:
    path = r"\\V8NETDRIVE\home\Regulator Testing Data"

    with cd(path):
        g = G(print_lines=False,
            aerotech_include=False,
            direct_write=True,
            direct_write_mode='serial',
            printer_port='COM18')
        
        task1 = AnalogInputTask()
        task1.create_voltage_channel('Dev1/ai0', terminal='diff',
                                    min_val=-5.0, max_val=5.0)
        task1.configure_timing_sample_clock(rate=sample_rate)
        
        self.daq = DAQ(task1)
    
    
        for i in range(0, pressure_ramp_cycles):
            # Ensure solenoid is closed at start
            g.write('M42 P8 S0')
            g.write('M125 S40')                     # Set tank pressure to 40psi
    
            current_ramp_cycle = i
            # Interval added / subtracted to help make range function more intuitive
            for p in range(self.pressure_start, self.pressure_end + self.pressure_interval, self.pressure_interval):
                print(p)
                self.solenoid_pressure_test(p)
        
            for p in range(self.pressure_end, self.pressure_start - self.pressure_interval, -self.pressure_interval):
                print(p)
                self.solenoid_pressure_test(p)
        
        self.g.teardown()
        
        self.daq.display_test_results()
    
