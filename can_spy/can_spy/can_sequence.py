import pickle

from .can_packet import CanPacket


def parse_can_sequence_file(path: str) -> list[CanPacket]:
    "Parse pickled CanPacket structs from a file. Note: will throw FileNotFoundException if path is invalid"
    packets = []
    with open(path, 'rb') as f:
        while True:
            try:
                packets.append(pickle.load(f))
            except ValueError:
                print(f"Warning, CAN sequence not fully parsed due to corruption")
                break
            except EOFError:
                break
    
    return packets

