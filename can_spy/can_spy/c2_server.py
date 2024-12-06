import asyncio
import signal
import websockets

from .can_packet import CanPacket

class C2Server:
    """C2 server used to handle connection with hardware"""

    def __init__(self,
                 server_address: str = "0.0.0.0",
                 server_port: int = 3005,
                 client_handler = None):
        self.server_address = server_address
        self.server_port = server_port
        self.client_handler = client_handler or self._handle_client

    def serve_forever(self):
        try:
            loop = asyncio.get_event_loop()
            loop.run_until_complete(self.start_websocket_server())
        except KeyboardInterrupt:
            print("\nServer stopped manually.")
        finally:
            loop.close()

    async def start_websocket_server(self):
        stop_event = asyncio.Event()

        def shutdown_signal():
            print("\nServer stopped manually.")
            stop_event.set()

        # Handle Keyboard interrupt for graceful shutdown
        loop = asyncio.get_event_loop()
        loop.add_signal_handler(signal.SIGINT, shutdown_signal)
        loop.add_signal_handler(signal.SIGTERM, shutdown_signal)    

        # Start the server
        server = await websockets.serve(self._handle_client, self.server_address, self.server_port)
        print(f"WebSocket server is running on ws://{self.server_address}:{self.server_port}")

        try:
            await stop_event.wait()  # Wait until stop_event is set
        finally:
            server.close()
            await server.wait_closed()

    async def _handle_client(self, websocket):
        """Default client handler. Behavior is to simply print all CAN packets received."""
        async for message in websocket:
            if isinstance(message, bytes):
                print("Received binary message:")

                can_packet = CanPacket.deserialize(message)
                print(can_packet)
            else:
                print(f"Received text message: {message}")