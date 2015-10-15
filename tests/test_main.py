#! /usr/bin/env python

import unittest
import numpy as np
from mecode import G


g = G(
    print_lines=False,
    aerotech_include=False,
    direct_write=True,
    direct_write_mode='serial',
    #printer_port="/dev/tty.usbmodem1411",
    printer_port="COM14",
)

def read_profilometer(samples=1):
    samples = np.floor(np.log2(samples))
    g.write('M400')
    prof_val = int(g.write('M235 S{}'.format(samples), resp_needed=True)[3:-1])
    if prof_val > 10000:
        return None
    prof_val -= 5000
    return prof_val


class MarlinTestCase(unittest.TestCase):

    def test_M237(self):
        cycles = 2
        measurement_locations = [
            (25, 60),
            (95, 175),
            (165, 60),
        ]
        measurements = np.zeros(len(measurement_locations))
        for i in range(cycles):
            g.move(Z=1)
            g.write('G28')
            g.write('M237')
            g.write('G1 F9000')
            for j, location in enumerate(measurement_locations):
                print ".",
                g.abs_move(*location)
                g.write('G4 P300')
                measurements[j] = read_profilometer(samples=4)
            stdev = np.std(measurements)
            msg = 'Bed level standard deviation was larger than 15 microns'
            self.assertLess(stdev, 15, msg)

    def test_M218(self):
        # Assert offset echoed correctly
        g.write('G92 X0 Y0 Z0')
        g.write('M218 T1 X10 Y10 Z10')
        response = g.write('M218', resp_needed=True)
        self.assertIn('0.00,0.00,0.00 10.00,10.00,10.00', response)

        # Assert offset applied to current position on tool change
        response = g.write('M114', resp_needed=True)
        self.assertIn('X:0.00 Y:0.00 Z:0.00', response)
        g.write('T1')
        response = g.write('M114', resp_needed=True)
        self.assertIn('X:10.00 Y:10.00 Z:10.00', response)

        # Reset Offset Values and state
        g.write('M218 T1 X0 Y0 Z0')
        g.write('T0')

    def test_M380(self):
        # Assert Solenoid 0 Is Invalid if Active Extruder is 0
        g.write('T0')
        response = g.write('M380 V', resp_needed=True)
        self.assertIn('Invalid solenoid', response)
        
        # Assert Solenoid 1 Pin Status is High if Active Extruder is 1
        g.write('T1')
        response = g.write('M380 V', resp_needed=True)
        self.assertIn('Solenoid 1 Status: 1', response)

        # Assert Enabled Solenoid (T1) is disabled with M381
        response = g.write('M381 V', resp_needed=True)
        self.assertIn('Solenoid 1 Status: 0', response)
        g.write('T0')

        # Assert T Parameter of T1 enables Solenoid 1
        response = g.write('M380 T1 V', resp_needed=True)
        self.assertIn('Solenoid 1 Status: 1', response)

        # Assert T Parameter of T0 invokes error
        response = g.write('M380 T0 V')
        self.assertIn('T0 Invalid solenoid', response)
        g.write('M381')

        # Assert T Parameter of T2 invokes error (tool > extruder)
        response = g.write('M380 T2 V', resp_needed=True)
        self.assertIn('T2 Invalid solenoid', response)

        # Explicitly reset tool and solenoid status
        g.write('T0')
        g.write('M381')

if __name__ == '__main__':
    unittest.main()