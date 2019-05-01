import socket
import threading
import system
import time


# sys.argv[1] IP a conectarse
# sys.argv[2] numero de puerto a abrir
# sys.argv[3] numero de clientes a conectar


class Cliente:
	IP = 0
	puerto = 0
	cliente_numero = 0

	def __init__(self, c, a, ip, port, cn):
		sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.sock.bind((IP, int(puerto)))
		IP = ip
		puerto = port
		cliente_numero = cn
		
	def run():
		try:
			sock.connetct((IP, puerto))
		except:
			print("No se pudo conectar")

        while True:
            data = "Dato del cliente " + cliente_numero
            for connection in self.connections:
                connection.send(bytes(data))
            if not data:
                print("Cerrando conexion " + str(a[0]) + ":" + str(a[1]))
                self.connections.remove(c)
                c.close()
                break


class Administrador:
	clientes = []
	IP = 0
	puerto = 0
	cant_clientes = 0
	cont = 0

	def __init__(self, ip, port, cc):
		cant_clientes = cc
		puerto = port
		IP = ip

	def run(self):
		while cant_clientes > cont:
			cont += 1
			sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			self.sock.bind((IP, int(puerto)))
			c, a = self.sock.accept()
        	cThread = threading.Thread(target = self.handler, args=(c,a))
            cThread.daemon = True
            cThread.start()
            self.clientes.append(c)
            print("Abriendo conexion "+ cont + " en: " + str(a[0]) + ":" + str(a[1]))
            time.sleeps(0.5)

        while True
        	time.sleeps(2)

    def handler(self, c, a):
    	cliente = Cliente(self, c, a, IP, puerto, cont)

            


administrador = Administrador(sys.argv[1], sys.argv[2], sys.argv[3])
administrador.run()