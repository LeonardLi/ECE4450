from random import randint
import sys, traceback, threading, socket

from VideoStream import VideoStream
from RtpPacket import RtpPacket


class ServerWorker:
    SETUP = 'SETUP'
    PLAY = 'PLAY'
    PAUSE = 'PAUSE'
    TEARDOWN = 'TEARDOWN'

    INIT = 0
    READY = 1
    PLAYING = 2
    state = INIT

    OK_200 = 0
    FILE_NOT_FOUND_404 = 1
    CON_ERR_500 = 2

    clientInfo = {}

    def __init__(self, clientInfo):
        self.clientInfo = clientInfo

    def run(self):
        threading.Thread(target=self.recvRtspRequest).start()

    def recvRtspRequest(self):
        """Receive RTSP request from the client."""
        connSocket = self.clientInfo['rtspSocket'][0]
        while True:
            data = connSocket.recv(256)
            if data:
                print "Data received:\n" + data
                self.processRtspRequest(data)

    def processRtspRequest(self, data):
        """Process RTSP request sent from the client."""

        # Get the request type
        request = data.split('\n')
        line1 = request[0].split(' ')
        requestType = line1[0]

        # Get the media file name
        filename = line1[1]

        # Get the RTSP sequence number
        seq = request[1].split(' ')

        # Process SETUP request
        if requestType == self.SETUP:
            if self.state == self.INIT:
                # Update state
                print "processing SETUP\n"
                try:
                    self.clientInfo['videoStream'] = VideoStream(filename)
                    self.state = self.READY
                except IOError:
                    self.replyRtsp(self.FILE_NOT_FOUND_404, seq[1])

                # Generate a randomized RTSP session ID
                self.clientInfo['session'] = randint(100000, 999999)

                # Send RTSP reply
                self.replyRtsp(self.OK_200, seq[1])

                # Get the RTP/UDP port from the last line
                self.clientInfo['rtpPort'] = request[2].split(' ')[3]

                # Init the RTP/UDP socket
                self.clientInfo['rtpSocket'] = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

        # --------------
        # TO COMPLETE
        # --------------
        # Process PLAY request
        # ...
        elif requestType == self.PLAY:
            if self.state == self.READY:
                print 'PLAY INCOME'

                self.clientInfo['event'] = threading.Event()
                self.state = self.PLAYING
                threading.Thread(target=self.sendRtp).start()
                #self.sendRtp()
                # Send RTSP reply
                self.replyRtsp(self.OK_200, seq[1])

        # Process PAUSE request
        # ...
        elif requestType == self.PAUSE:
            if self.state == self.PLAYING:
                print 'PAUSE INCOME'
                self.state = self.READY
                self.clientInfo['event'].set() # send signal

                # Send RTSP reply
                self.replyRtsp(self.OK_200, seq[1])

        # Process TEARDOWN request
        elif requestType == self.TEARDOWN:
            if self.state == self.PLAYING:
                print 'TEARDOWN INCOME'
                self.state = self.INIT
                self.clientInfo['event'].set()
                # Send RTSP reply
                self.replyRtsp(self.OK_200, seq[1])
        else:
            return


    def sendRtp(self):
        """Send RTP packets over UDP."""
        while True:
            self.clientInfo['event'].wait(0.05)

            # Stop sending if request is PAUSE or TEARDOWN
            if self.clientInfo['event'].isSet():
                break

            data = self.clientInfo['videoStream'].nextFrame()
            if data:
                frameNumber = self.clientInfo['videoStream'].frameNbr()
                try:
                    address = self.clientInfo['rtspSocket'][1][0]
                    port = int(self.clientInfo['rtpPort'])
                    self.clientInfo['rtpSocket'].sendto(self.makeRtp(data, frameNumber), (address, port))
                except:
                    print "Connection Error"

    def makeRtp(self, payload, frameNbr):
        """RTP-packetize the video data."""

    # -------------
    # TO COMPLETE
    # -------------
    # Set the fields
    # ...

    # Create and encode the RTP packet
    # ...
        rtppacket = RtpPacket()
        rtppacket.encode(payload, frameNbr)


    # Return the RTP packet
    # ...
        return rtppacket.getPacket()

    def replyRtsp(self, code, seq):
        """Send RTSP reply to the client."""
        if code == self.OK_200:
            # print "200 OK"
            reply = 'RTSP/1.0 200 OK\nCSeq: ' + seq + '\nSession: ' + str(self.clientInfo['session'])
            connSocket = self.clientInfo['rtspSocket'][0]
            connSocket.send(reply)

        # Error messages
        elif code == self.FILE_NOT_FOUND_404:
            print "404 NOT FOUND"
        elif code == self.CON_ERR_500:
            print "500 CONNECTION ERROR"
