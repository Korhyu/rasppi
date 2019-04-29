import socket
import threading
import sys


class Server:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    connections = []
    
    def __init__(self):
        self.sock.bind(('0.0.0.0', 10000))
        self.sock.listen(1)

    def handler(self, c, a):
        while True:
            data = c.recv(1024)
            if data == 0:
                print("Cerrando conexiones")
                self.closeall()
            
            for connection in self.connections:
                connection.send(bytes(data))
            if not data:
                connection.remove(c)
                c.close()
                break
                
    def run(self):
        while True:
            c, a = self.sock.accept()
            cThread = threading.Thread(target = self.handler, args=(c,a))
            cThread.daemon = True                            #Me permite cerrar el programa por mas que los threads esten abiertos
            cThread.start()
            self.connections.append(c)
            print(self.connections)
            
    def closeall(self)
        for connection in connections:
            print("Cerrando conexion {c}")
            connection.remove(c)
            c.close()
            thread.exit()
     
            
            
            
server = Server()
server.run()




#if data == startswith("exit"):
#    for connection in connections:
#        print("Cerrando conexion {c}")
#        connection.remove(c)
#        c.close()
#        thread.exit()
            



