import struct

from __future__ import annotations
from dataclasses import dataclass

@dataclass
class CanPacket:
    """Represents a single can_packet"""
    data: bytes
    timestamp: int
    can_id: int
    length: int

    def serialize(self) -> bytes:
        if self.length != len(self.data):
            raise ValueError("Length of `data` does not match `length` field")

        format_string = f'<L l I {self.length}s'
        return struct.pack(
            format_string,
            self.timestamp,
            self.can_id,
            self.length,
            self.data
        )

    @classmethod
    def deserialize(cls, buffer: bytes) -> CanPacket:
        marker = 0
        timestamp, = struct.unpack_from('<L', buffer, marker)
        marker += 4  # unsigned long

        can_id, = struct.unpack_from('<l', buffer, marker)
        marker += 4 # long

        length, = struct.unpack_from('<I', buffer, marker)
        marker += 4 # size_t

        data_format = f'{length}s'
        data, = struct.unpack_from(data_format, buffer, marker)

        return cls(
            data=data,
            timestamp=timestamp,
            can_id=can_id,
            length=length
        )