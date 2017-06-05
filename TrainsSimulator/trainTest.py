import socket
import time
import sys

HOST = "127.0.0.1"
if(len(sys.argv) == 2):
	HOST = sys.argv[1]      # Endereco IP do Servidor
PORT = 50002             # Porta que o Servidor esta
print("Hosting on " + HOST)
tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
dest = (HOST, PORT)
tcp.connect(dest)
commands = list()
#commands.append("\nA\nA\nA\n")
commands.append("GOTO ID RAILX\n")
commands.append("REG ID0 -RAIL1 RAIL2 -RAIL3 RAIL4\n");
commands.append("POS ID RAILX MAX\n")
commands.append("POS ID RAILX MIN\n")
commands.append("POS ID RAILX Xw\n")
commands.append("REG ID1 -RAIL1 RAIL2 -RAIL3 RAIL4\n");
commands.append("ALLOW ID LENGTH1 LENGTH2 LENGTH3 LENGTH4\n");
commands.append("DENY ID\n");
for cmd in commands:
	time.sleep(1)
	msg = cmd
	print("Sending: " + msg)
	tcp.send(msg.encode())

time.sleep(5)
tcp.close()
