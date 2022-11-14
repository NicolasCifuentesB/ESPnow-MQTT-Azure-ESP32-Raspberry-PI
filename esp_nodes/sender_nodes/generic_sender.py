import network
import espnow

# A WLAN interface must be active to send()/recv()
sta = network.WLAN(network.STA_IF)  # Or network.AP_IF
sta.active(True)

e = espnow.ESPNow()
e.active(True)
peer = b'\x08:\xf2\xb9\xa6\x0c'   # MAC address of peer's wifi interface
e.add_peer(peer)

e.send("Starting...")       # Send to all peers
for i in range(100):
    print(i)
    e.send(peer, str(i), True)
e.send(b'end')