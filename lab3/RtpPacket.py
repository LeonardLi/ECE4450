import sys
from time import time

HEADER_SIZE = 12


class RtpPacket:
    header = bytearray(HEADER_SIZE)

    def __init__(self):
        pass

    # --------------
    # TO COMPLETE
    # --------------
    # Fill in the input arguments if needed
    def encode(self, payload, frameNbr):
        """Encode the RTP packet with header fields and payload."""
        timestamp = int(time())
        header = bytearray(HEADER_SIZE)

    # Fill the header bytearray with RTP header fields
    # ...
        # version
        header[0] |= 1 << 7
        # P,X,CC,M all to 0

        # PT MJPEG is 26
        header[1] |= 0x1A

        # SeqNum
        header[3] |= frameNbr

        # Set timeStamp
        header[4] = (timestamp >> 24 ) & 0xFF
        header[5] = (timestamp >> 16 ) & 0xFF
        header[6] = (timestamp >> 8 ) & 0xFF
        header[7] = timestamp  & 0xFF

        # set SSRC any value I want  1024
        header[9] |= 0x10

        self.header = header
    # Get the payload
    # ...
        self.payload = payload

    def decode(self, byteStream):
        """Decode the RTP packet."""
        self.header = bytearray(byteStream[:HEADER_SIZE])
        self.payload = byteStream[HEADER_SIZE:]

    def version(self):
        """Return RTP version."""
        return int(self.header[0] >> 6)

    def seqNum(self):
        """Return sequence (frame) number."""
        seqNum = self.header[2] << 8 | self.header[3]
        return int(seqNum)

    def timestamp(self):
        """Return timestamp."""
        timestamp = self.header[4] << 24 | self.header[5] << 16 | self.header[6] << 8 | self.header[7]
        return int(timestamp)

    def payloadType(self):
        """Return payload type."""
        pt = self.header[1] & 127
        return int(pt)

    def getPayload(self):
        """Return payload."""
        return self.payload

    def getPacket(self):
        """Return RTP packet."""
        return self.header + self.payload
