#!/usr/bin/python3

from can_spy.can_spy import CanSpy
from can_spy.exploits.replay import Replay


def main():
    device = CanSpy()
    print(device.add(1, 2))

    replay_attack = Replay()
    replay_attack.start()


if __name__ == "__main__":
    main()
