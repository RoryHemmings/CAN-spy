#!/usr/bin/python3

import asyncio
import binascii
import pickle
import random
import signal
import struct
import websockets

from dataclasses import dataclass
from datetime import datetime

from can_spy.can_spy import CanSpy
from can_spy.exploits.replay import Replay


WS_SERVER_PORT = 3005


@dataclass
class CanPacket:
    """Represents a single can_packet"""
    data: bytes
    timestamp: int
    can_id: int
    length: int

def deserialize_can_packet(buffer: bytes) -> CanPacket:
    marker = 0
    timestamp, = struct.unpack_from('<L', buffer, marker)
    marker += 4  # unsigned long

    can_id, = struct.unpack_from('<l', buffer, marker)
    marker += 4 # long

    length, = struct.unpack_from('<I', buffer, marker)
    marker += 4 # size_t

    data_format = f'{length}s'
    data, = struct.unpack_from(data_format, buffer, marker)

    return CanPacket(
        data=data,
        timestamp=timestamp,
        can_id=can_id,
        length=length
    )

def serialize_can_packet(packet: CanPacket) -> bytes:
    if packet.length != len(packet.data):
        raise ValueError("Length of `data` does not match `length` field")

    format_string = f'<L l I {packet.length}s'
    return struct.pack(
        format_string,
        packet.timestamp,
        packet.can_id,
        packet.length,
        packet.data
    )

def parse_saved_file(path: str) -> list[CanPacket]:
    packets = []
    with open(path, 'rb') as f:
        while True:
            try:
                packets.append(pickle.load(f))
            except EOFError:
                break

    return packets

async def handle_client(websocket):
    # file_timestamp = datetime.now().strftime("%Y%m%d_%H%M%S%f")
    # filename = f"data_{file_timestamp}.can"
    # output_file = open(filename, 'ab')

    # async for message in websocket:
    #     if isinstance(message, bytes):
    #         print("Received binary message:")

    #         can_packet = deserialize_can_packet(message)
    #         print(can_packet)
    #         pickle.dump(can_packet, output_file)
    #     else:
    #         print(f"Received text message: {message}")

    # output_file.close()

    # angle = random.randint(15, 60)
    angle = 30
    while True:
        print("Set angle to 30")
        packet = CanPacket(
            data=angle.to_bytes(1, byteorder='little', signed=True),
            timestamp=100, # Arbitrary in this case
            can_id=0x10,
            length=1
        )

        await websocket.send(serialize_can_packet(packet))
        await asyncio.sleep(0.5)

async def start_websocket_server():
    stop_event = asyncio.Event()

    def shutdown_signal():
        print("\nServer stopped manually.")
        stop_event.set()

     # Handle Keyboard interrupt for graceful shutdown
    loop = asyncio.get_event_loop()
    loop.add_signal_handler(signal.SIGINT, shutdown_signal)
    loop.add_signal_handler(signal.SIGTERM, shutdown_signal)    

    # Start the server
    server = await websockets.serve(handle_client, "0.0.0.0", WS_SERVER_PORT)
    print(f"WebSocket server is running on ws://0.0.0.0:{WS_SERVER_PORT}")

    try:
        await stop_event.wait()  # Wait until stop_event is set
    finally:
        server.close()
        await server.wait_closed()

def main():
    """CLI script entry point"""
    # device = CanSpy()
    # print(device.add(1, 2))

    # replay_attack = Replay()
    # replay_attack.start()

    try:
        loop = asyncio.get_event_loop()
        loop.run_until_complete(start_websocket_server())
    except KeyboardInterrupt:
        print("\nServer stopped manually.")
    finally:
        loop.close()
    
if __name__ == "__main__":
    main()