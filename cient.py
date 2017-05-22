
import logging
import asyncio

from aiocoap import *

host = "coap://[fe80::78f0:6d4f:5287:432a%lowpan0]"
port = 5683
path = "temperature"
mUri = host + "/" + path

logging.basicConfig(level=logging.INFO)

@asyncio.coroutine
def main():
    protocol = yield from Context.create_client_context()

    request = Message(code=GET, uri=mUri)

    try:
        response = yield from protocol.request(request).response
    except Exception as e:
        print('Failed to fetch resource:')
        print(e)
    else:
        print('Result: %s\n%r'%(response.code, response.payload))

if __name__ == "__main__":
    asyncio.get_event_loop().run_until_complete(main())

