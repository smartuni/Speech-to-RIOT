
import logging
import asyncio
import os
import subprocess
from aiocoap import *

coap_prefix = "coap://"
host = "[fe80::78f0:6d4f:5287:432a%lowpan0]"
port = 5683
path = ".well-known/core"
mUri = coap_prefix + host + "/" + path

logging.basicConfig(level=logging.INFO)


async def main():
    protocol = yield from Context.create_client_context()
    request = Message(code=GET, uri=mUri, mtype=1)
    try:
        protocol_request = protocol.request(request)
        response = yield from protocol_request.response
        print('Result: %s\n%r'%(response.code, response.payload))
    except Exception as e:
        print('Failed to fetch resource:')
        print(e)
    else:
        print('Result: %s\n%r'%(response.code, response.payload))

if __name__ == "__main__":
    asyncio.get_event_loop().run_until_complete(main())
