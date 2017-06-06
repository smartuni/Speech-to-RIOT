import asyncio
import logging
import sys
from aiocoap import *

args = sys.argv

#command is one of [LOW,MEDIUM,FAST,OFF]
async def setFan(ipaddress, fanid, command):

    logging.basicConfig(level=logging.INFO)

    protocol = await Context.create_client_context()
    payload = '{"rpm" : ' + command + '}'

    request = Message(code=GET, uri='coap://[' + ipaddress + '%lowpan0]/fans/' + fanid, payload=payload)

    protocol.request(request)

setFan(args[1],args[2],args[3])

