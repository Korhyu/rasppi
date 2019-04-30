import socket
import threading
import sys

# sys.argv[1]           puerto del socket


class Server:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    connections = []
    
    def __init__(self, puerto):
        self.sock.bind(('0.0.0.0', int(puerto)))
        self.sock.listen(1)

    def handler(self, c, a):
        while True:
            data = c.recv(1024)
            for connection in self.connections:
                connection.send(bytes(data))
            if not data:
                print("Cerrando conexion " + str(a[0]) + ":" + str(a[0]))
                c.close()
            break
        
                
    def run(self):
        while True:
            c, a = self.sock.accept()
            cThread = threading.Thread(target = self.handler, args=(c,a))
            cThread.daemon = True                            #Me permite cerrar el programa por mas que los threads esten abiertos
            cThread.start()
            self.connections.append(c)
            print("Abriendo conexion " + str(a[0]) + ":" + str(a[0]))
            
#    def close_all(self):
#        for connection in connections:
#            print("Cerrando conexion {c}")
#            connection.remove(c)
#            c.close()
#            thread.exit()

            
server = Server(sys.argv[1])
server.run()


