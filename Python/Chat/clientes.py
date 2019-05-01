import socket
import threading
import system
import time


# sys.argv[1] numero de puerto a abrir
# sys.argv[2] numero de clientes a conectar

class Cliente (IP, puerto):
	

	def __init__(self, c, a):
		sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.sock.bind((IP, int(puerto)))
			
		try:
			sock.connetct((IP, puerto))
		except:
			print("No se pudo conectar")

        while True:
            data = c.recv(1024)
            for connection in self.connections:
                connection.send(bytes(data))
            if not data:
                print("Cerrando conexion " + str(a[0]) + ":" + str(a[1]))
                self.connections.remove(c)
                c.close()
                break


class Administrador:
	clientes = []
	cant_clientes = 0
	puerto = 0


	def __init__(self, port, cc):
		cant_clientes = cc
		puerto = port


	def run(self):
		cont = 0
		while cant_clientes > cont:
			sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			self.sock.bind(('0.0.0.0', int(puerto)))
			c, a = self.sock.accept()
        	cThread = threading.Thread(target = self.handler, args=(c,a))
            cThread.daemon = True
            cThread.start()
            self.clientes.append(c)
            print("Abriendo conexion "+ cont + " en: " + str(a[0]) + ":" + str(a[1]))
            cont++
            time.sleeps(0.5)

        while True
        	time.sleeps(1)

    def handler(self, c, a):
    	cliente = Cliente(self, c, a)

            


administrador = Administrador(sys.argv[1], sys.argv[2])
administrador.run()