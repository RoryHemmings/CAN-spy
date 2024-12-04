#!/usr/bin/python3

import asyncio
import websockets

from can_spy.can_spy import CanSpy
from can_spy.exploits.replay import Replay


async def connect_to_websocket():
    # uri = "wss://10.0.0.1:81"
    uri = "ws://10.0.0.1:81"

    try:
        async with websockets.connect(uri) as websocket:
            print("Connected to WebSocket server")

            # Send a message
            await websocket.send("Hello, WebSocket!")
            print("Message sent: Hello, WebSocket!")

            # Receive a response
            response = await websocket.recv()
            print(f"Response received: {response}")

    except websockets.exceptions.ConnectionClosed as e:
        print(f"Connection closed: {e}")
    except Exception as e:
        print(f"An error occurred: {e}")


def main():
    device = CanSpy()
    print(device.add(1, 2))

    replay_attack = Replay()
    replay_attack.start()

    asyncio.run(connect_to_websocket())


if __name__ == "__main__":
    main()
