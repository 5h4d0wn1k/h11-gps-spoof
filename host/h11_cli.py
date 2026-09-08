#!/usr/bin/env python3
"""H11 - GPS Spoofing host helper: SIMULATION-ONLY NMEA crafting + checksum.
Never transmits on a live GPS feed from this script.
Educational/authorized own-lab use only (see README "IMPORTANT").
"""
import argparse
import os
import sys

MOD = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, MOD)
from hw_common import DEMO_TAG, read_target


def nmea_checksum(sentence):
    c = 0
    for ch in sentence:
        c ^= ord(ch)
    return "%02X" % c


def build(body):
    if body.endswith("*"):
        body = body[:-1]
    return "$" + body + "*" + nmea_checksum(body)


def is_valid(sentence):
    if not sentence.startswith("$") or "*" not in sentence:
        return False
    body, _, rest = sentence[1:].partition("*")
    cs = rest.split()[0][:2]
    return cs.upper() == nmea_checksum(body)


def analyze(text):
    return [l.strip() for l in text.splitlines() if l.strip()]


def run_demo():
    print("=== H11 NMEA sentence crafting (SIMULATION ONLY) ===")
    gg = build("GPGGA,120000.00,4041.3000,N,07346.7000,W,1,08,0.9,13.0,M,0.0,M,,*")
    rm = build("GPRMC,120000.00,A,4041.3000,N,07346.7000,W,0.0,0.0,*")
    print("  " + gg)
    print("  " + rm)
    print("  checksum valid for both: %s"
          % all(is_valid(s) for s in (gg, rm)))
    print(DEMO_TAG)
    return 0


def main(argv=None):
    p = argparse.ArgumentParser(
        description="H11 GPS spoofer - simulation-only NMEA validator")
    p.add_argument("--demo", action="store_true", help="offline demo (exit 0)")
    p.add_argument("--file", help="NMEA sentence log")
    args = p.parse_args(argv)
    if args.demo or not args.file:
        return run_demo()
    ok = 0
    for line in analyze(open(args.file).read()):
        valid = is_valid(line)
        ok += int(valid)
        print("%s valid=%s" % (line, valid))
    print("valid %d/%d" % (ok, len(analyze(open(args.file).read()))))
    return 0


if __name__ == "__main__":
    sys.exit(main())
