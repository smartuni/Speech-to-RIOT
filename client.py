import sys
import logging
import asyncio

from aiocoap import *

logging.basicConfig(level=logging.INFO)

service_path = "coap://[fe80::68c0:6d50:52ae:432a%lowpan0]/temperature"

async def client():
    protocol = await Context.create_client_context()

    request = Message(code=GET, uri=service_path)

    try:
        response = await protocol.request(request).response
    except Exception as e:
        print('Failed to fetch resource:')
        print(e)
    else:
        print('Result: %s\n%r'%(response.code, response.payload))

if __name__ == "__main__":
    asyncio.get_event_loop().run_until_complete(client())
