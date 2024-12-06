import asyncio
import signal
import websockets

from can_packet import CanPacket

class CanSpy:
    """C2 server used to handle connections with the hardware"""

    def __init__(self,
                 server_address: str = "0.0.0.0",
                 server_port: int = 3005):
        self.server_address = server_address
        self.server_port = server_port

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

        # angle = random.randint(15, 60)
        # angle = 30
        # while True:
        #     print("Set angle to 30")
        #     packet = CanPacket(
        #         data=angle.to_bytes(1, byteorder='little', signed=True),
        #         timestamp=100, # Arbitrary in this case
        #         can_id=0x10,
        #         length=1
        #     )

        #     await websocket.send(packet.serialize())
        #     await asyncio.sleep(0.5)

    