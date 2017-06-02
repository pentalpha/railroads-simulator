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
commands.append("\nA\nA\nA\n")
commands.append("GOTO_ID_RAILX\n")
commands.append("POS_ID_RAILX_MAX\n")
commands.append("POS_ID_RAILX_MIN\n")
commands.append("POS_ID_RAILX_Xw\n")
commands.append("REG_ID_-RAIL1_RAIL2_-RAIL3_RAIL4\n");
commands.append("ALLOW_ID_LENGTH1_LENGTH2_LENGTH3_LENGTH4\n");
commands.append("DENY_ID\n");
for cmd in commands:
	time.sleep(0.2)
	msg = cmd
	print("Sending: " + msg)
	tcp.send(msg.encode())
tcp.close()
