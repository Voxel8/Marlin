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
    printer_port="COM6",
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
        measurements = np.zeros((cycles, len(measurement_locations)))
        for i in range(cycles):
            g.move(Z=1)
            g.write('G28')
            g.write('G237')
            g.write('G1 F9000')
            for j, location in enumerate(measurement_locations):
                print ".",
                g.abs_move(*location)
                g.write('G4 P300')
                measurements[i, j] = read_profilometer(samples=4)
        diff = measurements.max() - measurements.min()
        msg = "Bed level measurements differed by more than 10 microns"
        self.assertLess(diff, 10, msg)


if __name__ == '__main__':
    unittest.main()
