import subprocess, re
import asyncio

from aiocoap import *


def scanIps():
    ips = set()
    result = subprocess.run(['ping6', '-c', '5', 'ff02::1%lowpan0'], stdout=subprocess.PIPE)

    regex = r"\d* bytes from ([0-9,a-f]*::[0-9,a-f]*:[0-9,a-f]*:[0-9,a-f]*:[0-9,a-f]*): icmp_seq=\d* ttl=\d* time=\d*.\d*"

    matches = re.finditer(regex, str(result.stdout))

    for matchNum, match in enumerate(matches):
        matchNum = matchNum + 1

        for groupNum in range(0, len(match.groups())):
            groupNum = groupNum + 1
            ip = match.group(groupNum)
            ips.add(ip)

    return ips


async def main(ip):
    protocol = await Context.create_client_context()

    request = Message(code=GET, uri='coap://[' + ip + '%lowpan0]/.well-known/core')

    try:
        response = await protocol.request(request).response
    except Exception as e:
        print('Failed to fetch resource:')
        print(e)
    else:
        return response.payload


def writeToHostFans(ip, endpoint):
    regex = r"<\/fans\/0>,<\/fans\/1>"
    matches = re.finditer(regex, endpoint)

    for matchNum, match in enumerate(matches):
        matchNum += 1
        match = match.group()
        if match is not None:
            f = open("hostnamesFans.json", "w")
            f.write('{ "fan" : "' + ip + '"}')
            f.close()

def writeToHostTemperature(ip, endpoint):
    regex = r"<\/temperature>"
    matches = re.finditer(regex, endpoint)

    for matchNum, match in enumerate(matches):
        matchNum += 1
        match = match.group()
        if match is not None:
            f = open("hostnamesTemperature.json", "w")
            f.write('{ "temperature" : "' + ip + '"}')
            f.close()

ipset = scanIps()
print(ipset)
for ip in ipset:
    result = asyncio.get_event_loop().run_until_complete(main(ip))
    print(str(result) + ' \n')
    writeToHostFans(ip, str(result))
    writeToHostTemperature(ip, str(result))
