import socket
import threading
import sys
import signal
import time

# sys.argv[1]           puerto del socket


class Server:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    connections = []
    
    def __init__(self, puerto):
        self.sock.bind(('0.0.0.0', int(puerto)))
        self.sock.listen(1)
        
    def __del__(self):
        signal.signal(signal.SIGTERM, self.__del__)
        signal.signal(signal.SIGINT, self.__del__)
        print("Cerrando servidor")
        for connection in self.connections:
            print("Cerrando conexion " + str(a[0]) + ":" + str(a[1]))
            self.close()
            connection.remove(c)

    def handler(self, c, a):
        while True:
            data = c.recv(1024)
            for connection in self.connections:
                connection.send(bytes(data))
            if not data:
                print("Cerrando conexion " + str(a[0]) + ":" + str(a[1]))
                self.connections.remove(c)
                c.close()
                break
                
    def run(self):
        while True:
            c, a = self.sock.accept()
            cThread = threading.Thread(target = self.handler, args=(c,a))
            cThread.daemon = True
            cThread.start()
            self.connections.append(c)
            print("Abriendo conexion " + str(a[0]) + ":" + str(a[1]))
            
#    def close_all(self):
#        for connection in connections:
#            print("Cerrando conexion " + str(a[0]) + ":" + str(a[1]))
#            connection.remove(c)
#            c.close()
#            thread.exit()

            
server = Server(sys.argv[1])
server.run()


