import subprocess, re
def scanIps():
    ips = set()
    result = subprocess.run(['ping', '-c', '4', 'localhost'], stdout=subprocess.PIPE)
    regex = r"\d* bytes from (\d*.\d*.\d*.\d): icmp_seq=\d* ttl=\d* time=\d*.\d* ms"

    matches = re.finditer(regex, str(result.stdout))

    for matchNum, match in enumerate(matches):
        matchNum = matchNum + 1

        for groupNum in range(0, len(match.groups())):
            groupNum = groupNum + 1
            ip=match.group(groupNum)
            print("{group}".format(group=match.group(groupNum)))
            ips.add(ip)

    return ips

print(scanIps())