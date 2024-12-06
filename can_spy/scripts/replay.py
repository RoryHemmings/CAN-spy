#!/usr/bin/python3

from can_spy.c2_server import C2Server


def main():
    """Replay Attack example script"""

    c2 = C2Server()
    c2.serve_forever()
        
if __name__ == "__main__":
    main()