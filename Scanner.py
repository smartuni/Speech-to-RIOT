import subprocess, re
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
            ip=match.group(groupNum)
            print("{group}".format(group=match.group(groupNum)))
            ips.add(ip)

    return ips

print(scanIps())
