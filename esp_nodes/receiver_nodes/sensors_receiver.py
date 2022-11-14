import network, espnow

# A WLAN interface must be active to send()/recv()
sta = network.WLAN(network.STA_IF)
sta.active(True)

e = espnow.ESPNow()
e.active(True)

while True :
    host, msg = e.recv()
    if msg:             # msg == None if timeout in recv()
        print(msg.decode('utf-8'))
        if msg == b'end':
            print('DONE')
            break
        elif host.decode('utf-8') == 'dht sender mac' :
            print('Temperature: {}'.format(msg.decode('utf-8')))
        elif host.decode('utf-8') == 'ldr sender mac' :
            print('Luminosity: {}'.format(msg.decode('utf-8')))
        elif host.decode('utf-8') == 'hygrometer sender mac' :
            print('Hygrometry: {}'.format(msg.decode('utf-8')))