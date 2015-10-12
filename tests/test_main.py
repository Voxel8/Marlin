#! /usr/bin/env python

import unittest
import sys
from optparse import OptionParser
import numpy as np
from mecode import G

parser = OptionParser()
parser.add_option("-p", "--port", dest="com_port",
                  help="Device port value", metavar="PORT")

(options, args) = parser.parse_args()

if not options.com_port:
    parser.error('Port not given')

g = G(
    print_lines=False,
    aerotech_include=False,
    direct_write=True,
    direct_write_mode='serial',
    #printer_port="/dev/tty.usbmodem1411",
    printer_port=options.com_port,
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
        cycles = 10
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
                g.write('G4 P500')
                measurements[j] = read_profilometer(samples=4)
            stdev = np.std(measurements)
            msg = "Bed level standard deviation was larger than 15 microns"
            self.assertLess(stdev, 15, msg)


if __name__ == '__main__':
    unittest.main()