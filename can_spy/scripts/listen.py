#!/usr/bin/python3

from can_spy.can_packet import CanPacket
from can_spy.c2_server import C2Server


async def handle_client(websocket):
    # TODO make flag to dump this
    # file_timestamp = datetime.now().strftime("%Y%m%d_%H%M%S%f")
    # filename = f"data_{file_timestamp}.can"
    # output_file = open(filename, 'ab')

    async for message in websocket:
        if isinstance(message, bytes):
            print("Received binary message:")

            can_packet = CanPacket.deserialize(message)
            print(can_packet)
            # pickle.dump(can_packet, output_file)
        else:
            print(f"Received text message: {message}")

    # output_file.close()

def main():
    """CAN listening example"""

    c2 = C2Server(client_handler=handle_client)
    c2.serve_forever()
        
if __name__ == "__main__":
    main()