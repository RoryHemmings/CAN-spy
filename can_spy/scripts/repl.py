#!/usr/bin/python3

from can_spy.can_spy import CanSpy
from can_spy.exploits.replay import Replay


def main():
    """CLI script entry point"""

    c2 = CanSpy()
    c2.serve_forever()
        
if __name__ == "__main__":
    main()