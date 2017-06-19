import logging
import asyncio
import json

from aiocoap import *

logging.basicConfig(level=logging.INFO)
file = open("hostnamesTemperature.json")
for line in file:
    content = json.loads(line)
    hostname = content["temperature"]

async def main():
    protocol = await Context.create_client_context()

    request = Message(code=GET, uri='coap://[' + hostname + '%lowpan0]/temperature', mtype=NON)

    try:
        response = await protocol.request(request).response
    except Exception as e:
        print('Failed to fetch resource:')
        print(e)
    else:
        print(response.payload)

if __name__ == "__main__":
    asyncio.get_event_loop().run_until_complete(main())
