import socket
import threading
import system
import time

# print sys.argv[0] # prints python_script.py
# print sys.argv[1] # prints var1
# print sys.argv[2] # prints var2

# sys.argv[1] numero de puerto a abrir
# sys.argv[2] numero de clientes a conectar

class Cliente (IP, puerto):
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	#self.sock.bind(('0.0.0.0', int(puerto)))
	try:
		sock.connetct((IP, puerto))
	except:
		print("No se pudo conectar")
	
class Administrador:
	clientes = []
	
	
	
	
	
	def handler(self, c, a):
