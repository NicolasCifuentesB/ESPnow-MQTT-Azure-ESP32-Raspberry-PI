import network, espnow, dht11
from machine import Pin, ADC
from time import sleep

# A WLAN interface must be active to send()/recv()
sta = network.WLAN(network.STA_IF)  # Or network.AP_IF
sta.active(True)

e = espnow.ESPNow()
e.active(True)
peer = b'\x08:\xf2\xb9\xa6\x0c'   # MAC address of peer's wifi interface
e.add_peer(peer)

hygrometer = ADC(Pin(33))
hygrometer.atten(ADC.ATTN_11DB)

e.send("Starting...")       # Send to all peers
for i in range(100):
    try :
        print(hygrometer.read())
        e.send(peer, str(hygrometer.read()), True)
        sleep(2)
    except OSError as e :
        e.send(b'ERROR DE CENSADO')
e.send(b'end')