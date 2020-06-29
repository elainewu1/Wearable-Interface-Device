#udp server (Server Side)

import socketserver
import sys
import logging
import time

class MyUDPHandler(socketserver.BaseRequestHandler):
    """
    This class works similar to the TCP handler class, except that
    self.request consists of a pair of data and client socket, and since
    there is no connection the client address must be given explicitly
    when sending data back via sendto().
    """
        
    def handle(self):
        global decodeData
        global decodeData2
        data = self.request[0].strip()
        socket = self.request[1]
        #print("{} wrote:".format(self.client_address[0]))
        #print(data)
        decodeData = data.decode("ASCII").split(" ") 
        #print(decodeData)#//works
        socket.sendto(data.upper(), self.client_address)
'''
    def finish(self):
        self.logger.debug('finish')
   ''' 
if __name__ == "__main__":
    HOST, PORT = "192.168.137.1", 2390
    
    sum=0
    speed=0
    while True:
        server = socketserver.UDPServer((HOST, PORT), MyUDPHandler)
        server.handle_request()
        print(decodeData)
        avg = -2750
        sys.stdout.flush()
        server.server_close()
    #server.serve_forever()
    #server.shutdown()

#localhost
#192.
