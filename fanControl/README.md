PWM control gcoap

###
Type to flash device: BOARD=samr21-xpro make all flash term

###
create native taps
./../RIOT/dist/tools/tapsetup/tapsetup -c 2

## Example Use
This example uses gcoap as a server on RIOT native. Then we send a request from a libcoap example client on the Linux host.


### Verify setup from RIOT terminal

    > coap info

Expected response:

    CoAP server is listening on port 5683
     CLI requests sent: 0
    CoAP open requests: 0

### Query from libcoap example client
gcoap does not provide any output to the CoAP terminal when it handles a request. We recommend use of Wireshark to see the request and response. You also can add some debug output in the endpoint function callback.

    ./coap-client -N -m get -p 5683 coap://[fe80::1843:8eff:fe40:4eaa%tap0]/.well-known/core

Example response:

    v:1 t:NON c:GET i:0daa {} [ ]
    </cli/stats>

The response shows the endpoint registered by the gcoap CLI example.

### Send query to libcoap example server
Start the libcoap example server with the command below.

    ./coap-server

Enter the query below in the RIOT CLI.

    > coap get fe80::d8b8:65ff:feee:121b 5683 /.well-known/core

CLI output:

    gcoap_cli: sending msg ID 743, 75 bytes
    > gcoap: response Success, code 2.05, 105 bytes
    </>;title="General Info";ct=0,</time>;if="clock";rt="Ticks";title="Internal Clock";ct=0;obs,</async>;ct=0


[1]: https://tools.ietf.org/html/rfc7252    "CoAP spec"
[2]: https://github.com/RIOT-OS/RIOT/tree/master/examples/gnrc_networking    "instructions"
[3]: https://github.com/RIOT-OS/RIOT/tree/master/examples/gnrc_border_router    "SLIP instructions"
