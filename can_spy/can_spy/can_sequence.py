import pickle

from .can_packet import CanPacket


def parse_can_sequence_file(path: str) -> list[CanPacket]:
    packets = []
    with open(path, 'rb') as f:
        while True:
            try:
                packets.append(pickle.load(f))
            except EOFError:
                break

    return packets

