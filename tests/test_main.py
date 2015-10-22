#! /usr/bin/env python

import unittest
import numpy as np
from mecode import G


class MarlinTestCase(unittest.TestCase):

    def setUp(self):
        self.g = G(
            print_lines=False,
            aerotech_include=False,
            direct_write=True,
            direct_write_mode='serial',
            printer_port="/dev/tty.usbmodem1421",
            #printer_port="COM14",
        )

    def tearDown(self):
        # Make sure the previous test is fully finished before the next one
        # starts.
        self.g.write('M400', resp_needed=True)
        self.g.teardown()
        self.g = None

    def read_profilometer(self, samples=1):
        g = self.g
        samples = np.floor(np.log2(samples))
        g.write('M400')
        prof_val = int(g.write('M235 S{}'.format(samples), resp_needed=True)[3:-1])
        if prof_val > 10000:
            return None
        prof_val -= 5000
        return prof_val

    def test_M237(self):
        g = self.g
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
            g.write('G29')
            g.write('G1 F9000')
            for j, location in enumerate(measurement_locations):
                print ".",
                g.abs_move(*location)
                g.write('G4 P300')
                measurements[j] = self.read_profilometer(samples=4)
            stdev = np.std(measurements)
            msg = 'Bed level standard deviation {} is larger than 15 microns'.format(stdev)
            self.assertLess(stdev, 15, msg)

        # Home printer after M237 test
        g.write('G28')

    def test_M218(self):
        g = self.g
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
        g = self.g
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
        response = g.write('M380 T0 V', resp_needed=True)
        self.assertIn('T0 Invalid solenoid', response)
        g.write('M381')

        # Assert T Parameter of T2 invokes error (tool > extruder)
        response = g.write('M380 T2 V', resp_needed=True)
        self.assertIn('T2 Invalid solenoid', response)

        # Explicitly reset tool and solenoid status
        g.write('T0')
        g.write('M381')

    def test_M852(self):
        g = self.g
        # Assert startup state is as expected.
        current_position = g.write('M114', resp_needed=True)
        self.assertIn('X:0.00 Y:0.00 Z:0.00', current_position)
        orig_offset = float(g.write('M851', resp_needed=True).strip()[16:21])
        g.write('M851 Z-3')
        offset = g.write('M851', resp_needed=True)
        self.assertIn('-3.00', offset)

        # Move bed level position up 100 microns.
        g.write('G28')
        g.abs_move(Z=0.1)
        resp = g.write('M852 V', resp_needed=True)
        self.assertIn('-2.9', resp)

        # Calling again from the same position should have no effect.
        resp = g.write('M852 V', resp_needed=True)
        self.assertIn('-2.9', resp)

        # Marlin should think it is at 0 right now.
        current_position = g.write('M114', resp_needed=True)
        self.assertIn('Z:0.00', current_position)

        # Test the optional Z argument
        g.write('M852 Z1')
        current_position = g.write('M114', resp_needed=True)
        self.assertIn('Z:1.00', current_position)

        # Reset state
        g.write('M851 Z{}'.format(orig_offset))
        g.write('M500')
        g.move(Z=5)


if __name__ == '__main__':
    unittest.main()
