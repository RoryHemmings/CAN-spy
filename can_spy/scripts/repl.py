#!/usr/bin/python3

import asyncio
import binascii
import signal
import websockets

from can_spy.can_spy import CanSpy
from can_spy.exploits.replay import Replay


WS_SERVER_PORT = 3005


async def echo(websocket):
    async for message in websocket:
        if isinstance(message, bytes):
            print("Received binary message:")
            print(binascii.hexlify(message).decode('utf-8'))  # Hexdump the binary message
            # await websocket.send(f"Binary message received, length: {len(message)}")
        else:
            print(f"Received text message: {message}")
            # await websocket.send(f"Echo: {message}")

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
    server = await websockets.serve(echo, "0.0.0.0", WS_SERVER_PORT)
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