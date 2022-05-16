import socket
import time

def input_commands(commands,client):
    for dir in commands:
        #print(dir," ")
        com = bytes(str(dir), 'utf-8')
        #print(com)
        client.send(com)
        # print("command sent")


# input_commands(commands)
