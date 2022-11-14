import network, espnow, dht11
from machine import Pin, ADC

# A WLAN interface must be active to send()/recv()
sta = network.WLAN(network.STA_IF)  # Or network.AP_IF
sta.active(True)

e = espnow.ESPNow()
e.active(True)
peer = b'\x08:\xf2\xb9\xa6\x0c'   # MAC address of peer's wifi interface
e.add_peer(peer)

ldr = ADC(Pin(34))
ldr.atten(ADC.ATTN_11DB)

e.send("Starting...")       # Send to all peers
for i in range(100):
    try :
        print(ldr.read())
        e.send(peer, str(ldr.read()), True)
    except OSError as e :
        e.send(b'ERROR DE CENSADO')
e.send(b'end')