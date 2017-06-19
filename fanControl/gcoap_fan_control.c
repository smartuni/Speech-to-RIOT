/**
 * @file		gcoap_fan_contol.c
 * @brief       gcoap fan control support
 *
 * @author      Julian Magierski
 * Gcoap example :Ken Bannister <kb2ma@runbox.com>
 *
 * @}
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net/gcoap.h"
#include "jsmn.h"
#include "od.h"
#include "fmt.h"

extern int pwmFast(int argc);
extern int pwmMedium(int argc);
extern int pwmLow(int argc);
extern int offFan(int argc);
static void _resp_handler(unsigned req_state, coap_pkt_t* pdu);
static ssize_t _stats_handler(coap_pkt_t* pdu, uint8_t *buf, size_t len);
static ssize_t _fan0Handler(coap_pkt_t* pdu, uint8_t *buf, size_t len);
static ssize_t _fan1Handler(coap_pkt_t* pdu, uint8_t *buf, size_t len);

/* CoAP resources */
// https://tools.ietf.org/html/rfc7252 12.3.
// 0  = plain/text
// 40 = link-format
// 50 = json
// Body: {"rpm" : "LOW/MEDIUM/FAST/OFF"}
static const coap_resource_t _resources[] = {
    { "/cli/stats", COAP_GET, _stats_handler }, 
    { "/fans/0", COAP_PUT, _fan0Handler },
	{ "/fans/1", COAP_PUT, _fan1Handler },
};

static gcoap_listener_t _listener = {
    (coap_resource_t *)&_resources[0],
    sizeof(_resources) / sizeof(_resources[0]),
    NULL
};

/* Counts requests sent by CLI. */
static uint16_t req_count = 0;

/*
 * Response callback.
 */
static void _resp_handler(unsigned req_state, coap_pkt_t* pdu)
{
    if (req_state == GCOAP_MEMO_TIMEOUT) {
        printf("gcoap: timeout for msg ID %02u\n", coap_get_id(pdu));
        return;
    }
    else if (req_state == GCOAP_MEMO_ERR) {
        printf("gcoap: error in response\n");
        return;
    }

    char *class_str = (coap_get_code_class(pdu) == COAP_CLASS_SUCCESS)
                            ? "Success" : "Error";
    printf("gcoap: response %s, code %1u.%02u", class_str,
                                                coap_get_code_class(pdu),
                                                coap_get_code_detail(pdu));
    if (pdu->payload_len) {
        if (pdu->content_type == COAP_FORMAT_TEXT
                || pdu->content_type == COAP_FORMAT_LINK
                || coap_get_code_class(pdu) == COAP_CLASS_CLIENT_FAILURE
                || coap_get_code_class(pdu) == COAP_CLASS_SERVER_FAILURE) {
            /* Expecting diagnostic payload in failure cases */
            printf(", %u bytes\n%.*s\n", pdu->payload_len, pdu->payload_len,
                                                          (char *)pdu->payload);
        }
        else {
            printf(", %u bytes\n", pdu->payload_len);
            od_hex_dump(pdu->payload, pdu->payload_len, OD_WIDTH_DEFAULT);
        }
    }
    else {
        printf(", empty payload\n");
    }
}

/*
 * Server callback for /cli/stats. Returns the count of packets sent by the
 * CLI.
 */
static ssize_t _stats_handler(coap_pkt_t* pdu, uint8_t *buf, size_t len)
{
    gcoap_resp_init(pdu, buf, len, COAP_CODE_CONTENT);
    size_t payload_len = fmt_u16_dec((char *)pdu->payload, req_count);
    return gcoap_finish(pdu, payload_len, COAP_FORMAT_JSON);
}


/*
 * Server HTTP PUT call for /fans/0
 */
static ssize_t _fan0Handler(coap_pkt_t* pdu, uint8_t *buf, size_t len) {
	unsigned code = COAP_CODE_CHANGED;
	int r;
    ssize_t plen = 0;
    char rsp[16];
	char payload[32] = { '\0' };
	int length = 0;
	jsmn_parser p;
	jsmntok_t t[20]; /* number of tokens */
	jsmn_init(&p);
    memcpy(payload, (char*)pdu->payload, pdu->payload_len);
	printf("Payload MSG %s \n", payload);	
	length = pdu->payload_len;
	printf("Length : %d \n", pdu->payload_len);
	r = jsmn_parse(&p, (char*)pdu->payload, length, t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		 printf("Failed to parse JSON: %d\n", r);
	}
	 // /* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		 printf("Object expected\n");
		 return 1;
	 }
	 if (strncmp(payload + t[1].start, "rpm", 3) == 0) {
		 printf("- rpm: %.*s\n", t[2].end-t[2].start,(char*)pdu->payload + t[2].start);
		 if (strncmp(payload + t[2].start, "LOW", 3) == 0) {
			 printf("---- pwmLOW Nr. 1 \n");
			 pwmLow(1);
		 } else if (strncmp(payload + t[2].start, "MEDIUM", 6) == 0) {
			printf("---- pwmMEDIUM Nr. 1 \n");
			pwmMedium(1);
		 } else if (strncmp(payload + t[2].start, "FAST", 4) == 0) {
		printf("----- pwmFAST Nr. 1 \n");		
		pwmFast(1);
		 } else if (strncmp(payload + t[2].start, "OFF", 3) == 0) {
		printf("---- offFan Nr. 1 \n");
		 offFan(1);
		 }	
	 }  	  
    return coap_reply_simple(pdu, code, buf, len,
            COAP_FORMAT_TEXT, (uint8_t*)rsp, plen);
}

/*
 * Server HTTP PUT call for /fans/1
 */
static ssize_t _fan1Handler(coap_pkt_t* pdu, uint8_t *buf, size_t len) {
	unsigned code = COAP_CODE_CHANGED;
	int r;
    ssize_t plen = 0;
    char rsp[16];
	char payload[32] = { '\0' };
	int length = 0;
	jsmn_parser p;
	jsmntok_t t[20]; /* number of tokens */
	jsmn_init(&p);
    memcpy(payload, (char*)pdu->payload, pdu->payload_len);
	printf("Payload MSG %s \n", payload);	
	length = pdu->payload_len;
	printf("Length : %d \n", pdu->payload_len);
	r = jsmn_parse(&p, (char*)pdu->payload, length, t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		 printf("Failed to parse JSON: %d\n", r);
	}
	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		 printf("Object expected\n");
		 return 1;
	 }
	 if (strncmp(payload + t[1].start, "rpm", 3) == 0) {
		 printf("- rpm: %.*s\n", t[2].end-t[2].start,(char*)pdu->payload + t[2].start);
		 if (strncmp(payload + t[2].start, "LOW", 3) == 0) {
			 printf("---- pwmLOW Nr. 2 \n");
			 pwmLow(2);
		 } else if (strncmp(payload + t[2].start, "MEDIUM", 6) == 0) {
			printf("---- pwmMEDIUM Nr. 2 \n");
			pwmMedium(2);
		 } else if (strncmp(payload + t[2].start, "FAST", 4) == 0) {
		printf("----- pwmFAST Nr. 2 \n");		
		pwmFast(2);
		 } else if (strncmp(payload + t[2].start, "OFF", 3) == 0) {
		printf("---- offFan Nr. 2 \n");
		 offFan(2);
		 }	
	 }  	  
    return coap_reply_simple(pdu, code, buf, len,
            COAP_FORMAT_TEXT, (uint8_t*)rsp, plen);
}


static size_t _send(uint8_t *buf, size_t len, char *addr_str, char *port_str)
{
    ipv6_addr_t addr;
    size_t bytes_sent;
    sock_udp_ep_t remote;
    remote.family = AF_INET6;
    remote.netif  = SOCK_ADDR_ANY_NETIF;
    /* parse destination address */
    if (ipv6_addr_from_str(&addr, addr_str) == NULL) {
        puts("gcoap_cli: unable to parse destination address");
        return 0;
    }
    memcpy(&remote.addr.ipv6[0], &addr.u8[0], sizeof(addr.u8));

    /* parse port */
    remote.port = (uint16_t)atoi(port_str);
    if (remote.port == 0) {
        puts("gcoap_cli: unable to parse destination port");
        return 0;
    }
    bytes_sent = gcoap_req_send2(buf, len, &remote, _resp_handler);
    if (bytes_sent > 0) {
        req_count++;
    }
    return bytes_sent;
}

int gcoap_cli_cmd(int argc, char **argv)
{
    /* Ordered like the RFC method code numbers, but off by 1. GET is code 0. */
    char *method_codes[] = {"get", "post", "put"};
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    size_t len;
    if (argc == 1) {
        /* show help for main commands */
        goto end;
    }
    for (size_t i = 0; i < sizeof(method_codes) / sizeof(char*); i++) {
        if (strcmp(argv[1], method_codes[i]) == 0) {
            if (argc == 5 || argc == 6) {
                if (argc == 6) {
                    gcoap_req_init(&pdu, &buf[0], GCOAP_PDU_BUF_SIZE, i+1, argv[4]);
                    memcpy(pdu.payload, argv[5], strlen(argv[5]));
                    len = gcoap_finish(&pdu, strlen(argv[5]), COAP_FORMAT_TEXT);
                }
                else {
                    len = gcoap_request(&pdu, &buf[0], GCOAP_PDU_BUF_SIZE, i+1,
                                                                           argv[4]);
                }
                printf("gcoap_cli: sending msg ID %u, %u bytes\n", coap_get_id(&pdu),
                                                                   (unsigned) len);
                if (!_send(&buf[0], len, argv[2], argv[3])) {
                    puts("gcoap_cli: msg send failed");
                }
                return 0;
            }
            else {
                printf("usage: %s <get|post|put> <addr> <port> <path> [data]\n",
                                                                       argv[0]);
                return 1;
            }
        }
    }
    if (strcmp(argv[1], "info") == 0) {
        if (argc == 2) {
            uint8_t open_reqs;
            gcoap_op_state(&open_reqs);
            printf("CoAP server is listening on port %u\n", GCOAP_PORT);
            printf(" CLI requests sent: %u\n", req_count);
            printf("CoAP open requests: %u\n", open_reqs);
            return 0;
        }
    }
    end:
    printf("usage: %s <get|post|put|info>\n", argv[0]);
    return 1;
}

void gcoap_fan_control_init(void) {
    gcoap_register_listener(&_listener);
}
