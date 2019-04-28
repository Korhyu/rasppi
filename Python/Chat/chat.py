import socket
import threading
import sys

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sock.bind(('0.0.0.0', 12000))
sock.listen(1)


connections = []


def handler(c, a):
    global connections
    while True:
        data = c.recv(1024)
        if data == startswith("exit"):
            for connection in connections:
                print("Cerrando conexion {c}")
                connection.remove(c)
                c.close()
                thread.exit()
        for connection in connections:
            connection.send(bytes(data))
        if not data:
            connection.remove(c)
            c.close()
            break



while True:
    c, a = sock.accept()
    cThread = threading.Thread(target = handler, args=(c,a))
    cThread.daemon = True                            #Me permite cerrar el programa por mas que los threads esten abiertos
    cThread.start()
    connections.append(c)
    print(connections)
