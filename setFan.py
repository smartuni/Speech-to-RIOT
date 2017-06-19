import asyncio
import logging
import sys
from aiocoap import *

args = sys.argv

#command is one of [LOW,MEDIUM,FAST,OFF]
async def setFan(ipaddress, fanid, command):

    protocol = await Context.create_client_context()
    payload_to_send = '{"rpm" : ' + command + '}'

    request = Message(code=PUT, uri='coap://[' + ipaddress + '%lowpan0]/fans/' + fanid, payload=payload_to_send.encode())

    try:
        response = await protocol.request(request).response
        print ("sent request!")
    except Exception as e:
        print('Failed to fetch resource:')
        print(repr(e))
    else:
        print("Success")


    #logging.basicConfig(level=logging.INFO)
    #fan_id_as_int = int(fanid)

    #protocol = await Context.create_client_context()
    #payload = '{"rpm" : ' + command + '}'

    #request = Message(code=PUT, uri='coap://[' + ipaddress + '%lowpan0]/fans/' + fanid_as_int, payload=payload, mtype=NON)

    #protocol.request(request)

#setFan(args[1],args[2],args[3])
#setFan("fe80::7b62:3469:7bbd:d1be","0","MEDIUM")

if __name__ == "__main__":
    asyncio.get_event_loop().run_until_complete(setFan(args[1],args[2],args[3]))

