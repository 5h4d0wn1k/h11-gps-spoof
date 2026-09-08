import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "host"))
import h11_cli as m


class TestNmea(unittest.TestCase):
    def test_checksum_known(self):
        self.assertEqual(m.nmea_checksum("GPGGA,120000.00,4041.3000,N"), m.nmea_checksum("GPGGA,120000.00,4041.3000,N"))

    def test_build_and_validate(self):
        s = m.build("GPRMC,120000.00,A,4041.3000,N,07346.7000,W,0.0,0.0,*")
        self.assertTrue(s.startswith("$"))
        self.assertTrue(m.is_valid(s))

    def test_invalid(self):
        self.assertFalse(m.is_valid("not-a-sentence"))
        self.assertFalse(m.is_valid("$GPGGA*XX"))


if __name__ == "__main__":
    unittest.main()
