import network
import espnow

# A WLAN interface must be active to send()/recv()
sta = network.WLAN(network.STA_IF)
sta.active(True)

e = espnow.ESPNow()
e.active(True)

while True:
    host, msg = e.recv()
    if msg:             # msg == None if timeout in recv()
        print(msg.decode())
        if msg == b'end':
            print('DONE')
            break
        elif host == b'dht sender mac':
            print('Temperature: {}'.format(msg.decode()))
        elif host == b'ldr sender mac':
            print('Luminosity: {}'.format(msg.decode()))
        elif host == b'hygrometer sender mac':
            print('Hygrometry: {}'.format(msg.decode()))
