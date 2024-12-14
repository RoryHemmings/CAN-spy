#!/usr/bin/python3

import argparse
import datetime
import pickle

from can_spy.can_packet import CanPacket
from can_spy.c2_server import C2Server


async def handle_client(websocket, args: argparse.Namespace):
    # Generate output file path
    output_path = args.outfile
    if (output_file == None):
        file_timestamp = datetime.now().strftime("%Y%m%d_%H%M%S%f")
        output_path = f"data_{file_timestamp}.can"

    output_file = open(output_path, 'ab')

    # Perpetually listen for incoming messages
    async for message in websocket:
        if isinstance(message, bytes):
            print("Received binary message:")

            can_packet = CanPacket.deserialize(message)

            if (not args.no_stdout):
                print(can_packet)
            if (not args.no_file):
                pickle.dump(can_packet, output_file)
        else:
            print(f"Received text message: {message}")

    output_file.close()

def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Records and serializes websocket activity to a pickle file for later examination or use in replay attack.")

    parser.add_argument("-o", "--outfile",
                        type=str,
                        help="Path of the file to save recorded CAN activity to.")
    parser.add_argument("-s", "--no-stdout",
                        action="store_true",
                        help="Hides incoming CAN activity from stdout. Can help reduce latency.")
    parser.add_argument("-n", "--no-file",
                        action="store_true",
                        help="Prevents incoming CAN activity from being written to a file if present.")
    return parser.parse_args()

def main():
    """CAN listening example"""

    args = parse_arguments()

    client_handler = lambda websocket: handle_client(websocket, args)
    c2 = C2Server(client_handler=client_handler)
    c2.serve_forever()
        
if __name__ == "__main__":
    main()