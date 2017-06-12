import subprocess, re
import asyncio

from aiocoap import *


def scanIps():
    ips = set()
    result = subprocess.run(['ping6', '-c', '1', 'ff02::1%lowpan0'], stdout=subprocess.PIPE)

    regex = r"\d* bytes from ([0-9,a-f]*::[0-9,a-f]*:[0-9,a-f]*:[0-9,a-f]*:[0-9,a-f]*): icmp_seq=\d* ttl=\d* time=\d*.\d*"

    matches = re.finditer(regex, str(result.stdout))
    print(str(result.stdout))

    for matchNum, match in enumerate(matches):
        matchNum = matchNum + 1

        for groupNum in range(0, len(match.groups())):
            groupNum = groupNum + 1
            ip = match.group(groupNum)
            print("{group}".format(group=match.group(groupNum)))
            ips.add(ip)

    return ips

<<<<<<< HEAD
print(scanIps())
=======

print(scanIps())


async def main(ip):
    protocol = await Context.create_client_context()

    request = Message(code=GET, uri='coap://[' + ip + ']/.well-known/core', mtype=NON)

    try:
        response = await protocol.request(request).response
    except Exception as e:
        print('Failed to fetch resource:')
        print(e)
    else:
        print(response.payload)
        return response.payload


if __name__ == "__main__":
    asyncio.get_event_loop().run_until_complete(main())

ipset = scanIps()
for ip in ipset:
    result = main(ip)
    print(result + ' \n')
>>>>>>> 275be38d7f949b83d5eb2c4ca32dc80289abb55e
