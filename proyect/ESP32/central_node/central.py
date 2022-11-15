from time import sleep
from umqtt.simple import MQTTClient
import network
import espnow

sta = network.WLAN(network.STA_IF)
sta.active(True)

e = espnow.ESPNow()
e.active(True)

SERVER = '192.168.131.38'  # MQTT SERVER ADDRESS (raspberry)
CLIENT_ID = 'ESP32'
TOPIC_TEMP = b'temperature'
TOPIC_HYGRO = b'hygrometry'
TOPIC_LDR = b'luminosity'

client = MQTTClient(CLIENT_ID, SERVER)
client.connect()

while True:
    host, msg = e.recv()
    if msg:             # msg == None if timeout in recv()
        print(msg.decode())
        if msg == b'end':
            print('DONE')
            break
        elif host == b'dht sender mac':
            print('Temperature: {}'.format(msg.decode()))
            client.publish(TOPIC_TEMP, msg)
        elif host == b'ldr sender mac':
            print('Luminosity: {}'.format(msg.decode()))
            client.publish(TOPIC_LDR, msg)
        elif host == b'hygrometer sender mac':
            print('Hygrometry: {}'.format(msg.decode()))
            client.publish(TOPIC_HYGRO, msg)
