import asyncio
import logging
from aiocoap import *

#command is one of [LOW,MEDIUM,FAST,OFF]
def setFan(ipaddress, fanid, command):


    logging.basicConfig(level=logging.INFO)

    protocol = await Context.create_client_context()
    payload = '{"rpm" : ' + command + '}'

    request = Message(code=PUT, uri='coap://[' + ipaddress + '%lowpan0]/fans/' + fanid, payload=payload)

    protocol.request(request)