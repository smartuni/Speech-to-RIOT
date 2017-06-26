/**
 * Speech to Riot
 * @file        main.c
 * @brief       fan control application
 *
 * @author      Julian Magierski
 * Gcoap example Ken Bannister <kb2ma@runbox.com>
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "msg.h"

#include "net/gcoap.h"
#include "kernel_types.h"
#include "shell.h"
#include "xtimer.h"
#include "periph_conf.h"
#include "periph/gpio.h"
#include "periph/pwm.h"
#include "board.h"

#define MAIN_QUEUE_SIZE (4)
#include "timex.h"
#define STEPS (750U)
#define FREQU (1000U)
#define MODE PWM_LEFT

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

extern int gcoap_cli_cmd(int argc, char **argv);
extern void gcoap_fan_control_init(void);

/*
	set the specific fan nr. to medium pwm speed.
	@param int pwm nr.
*/
int pwmMedium(int argc) {
	printf("Medium main \n");
	switch(argc) {
	case 1:
		pwm_set(PWM_DEV(1), 1, 700); // duty cycle
		break;
	case 2:
		pwm_set(PWM_DEV(1), 2, 700);	
		break;
	default:
		printf("Function Medium fan: not known fan number \n");
	}
    return 0;
}

/*
	Shell pwmMedium command handler
	set the specific fan nr. to medium pwm speed.
*/
static int pwmMediumCmd(int argc, char **argv) {
	if (argc > 1) {
		int pwmNr = atoi(argv[1]);
		switch(pwmNr) {
		case 1:
			pwm_set(PWM_DEV(1), 1, 700);
			break;
		case 2:
			pwm_set(PWM_DEV(1), 2, 700);	
			break;
		default:
			printf("Function Medium fan: not known fan number \n");
			printf("usage: medium <number> \n");
		}
	}
    return 0;
}

/*
	set the specific fan nr. to low pwm speed.
	@param int fan nr.
*/
int pwmLow(int argc) {
	switch(argc) {
	case 1:
		pwm_set(PWM_DEV(1), 1, 400);
		break;
	case 2:
		pwm_set(PWM_DEV(1), 2, 400);	
		break;
	default:
		printf("Function Low fan: not known fan number \n");
	}
    return 0;
}

/*
	Shell pwmLow command handler
	set the specific fan nr. to low pwm speed.
*/
static int pwmLowCmd(int argc, char **argv) {
	if (argc > 1) {
		int pwmNr = atoi(argv[1]);
		switch(pwmNr) {
		case 1:
			pwm_set(PWM_DEV(1), 1, 400);
			break;
		case 2:
			pwm_set(PWM_DEV(1), 2, 400);	
			break;
		default:
			printf("Function Low fan: not known fan number \n");
			printf("usage: low <number> \n");
		}
	}
    return 0;
}

/**
change the specific fan nr. from low speed to fast.
*/
static int pwmLowtoFastCmd(int argc, char **argv) {
	int pwmNr = atoi(argv[1]);
	int steps = atoi(argv[2]);
	if (argc != 3) { // asstert the right number of arguments
		printf("usage: pwmLowtoFast <number|steps> \n");
	return 1;
	}
	if (pwmNr != 1 && pwmNr != 2) { // assert for correct pwmNr 
		printf("pwmNr. is not 1 or 2 \n");
		printf("usage: lowtoFast <number|steps> \n");
	return 2;
	}
	pwm_set(PWM_DEV(1), pwmNr, 0);
	xtimer_usleep(50000);
	for (int resolution=200; resolution<=900; resolution+= steps) {
		pwm_set(PWM_DEV(1), pwmNr, resolution);
		xtimer_usleep(200000);
	}	
    return 0;
}

/*
	Set the specific fan nr. to fast pwm speed.
	@param int fan nr.
*/
int pwmFast(int argc) {	
	switch(argc) {
	case 1:
		pwm_set(PWM_DEV(1), 1, 900);
		break;
	case 2:
		pwm_set(PWM_DEV(1), 2, 900);	
		break;
	default:
		printf("Function pwm fast: not known fan number \n");	
	}
	return 0;
}

/*
	Shell pwmFast command handler
	set the specific fan nr. to fast pwm speed.
*/
static int pwmFastCmd(int argc, char **argv) {	
	if (argc > 1) {	
		int pwmNr = atoi(argv[1]);
		switch(pwmNr) {
		case 1:
			pwm_set(PWM_DEV(1), 1, 900);
			break;
		case 2:
			pwm_set(PWM_DEV(1), 2, 900);	
			break;
		default:
		printf("Function pwmFast: not known fan number \n");
		printf("usage: fast <number> \n");
		}
	}
	return 0;
}

/**
* Set the specific fan nr. off
*/
int offFan(int argc) {
	switch(argc) {
	case 1:
		pwm_set(PWM_DEV(1), 1, 0);
		break;
	case 2:
		pwm_set(PWM_DEV(1), 2, 0);	
		break;
	default:
		printf("Function offFan: not known fan number \n");
	}
	return 0;
}

/*
	Shell offFan command handler
	set the specific fan nr. off
*/
static int offFanCmd(int argc, char **argv) {
	if (argc > 1) {
		int pwmNr = atoi(argv[1]);
		switch(pwmNr) {
		case 1:
			pwm_set(PWM_DEV(1), 1, 0);
			break;
		case 2:
			pwm_set(PWM_DEV(1), 2, 0);	
			break;
		default:
			printf("Function offFan: not known fan number \n");
			printf("usage: off <number> \n");
		}
	}
	return 0;
}

static const shell_command_t shell_commands[] = {
	{ "coap", "CoAP example", gcoap_cli_cmd },
	{ "low", "pwm Up", pwmLowCmd},            // Set fan nr. 1-2 to low speed
	{ "lowToFast", "from Low to Fast", pwmLowtoFastCmd},  // Set fan nr. 1-2 to low speed an change to Fast speed
	{ "medium", "pwm down", pwmMediumCmd },   // fan nr. 1-2 to medium speed
	{ "fast", "pwm Up", pwmFastCmd},          // fan nr. 1-2 to fast speed
	{ "off", "off Fan", offFanCmd},           // set off fan nr. 1-2
    { NULL, NULL, NULL }                         
};

/* 
	This is the main function.
	Here we init the pwm devices and start the shell command.
*/
int main(void) {  
	uint32_t pwm_Hz = 0;
    puts("----------__ Gcoap fan control application __----------");
	/*  init pwm devices */
	 for (int i = 0; i < 2; i++) {
        pwm_Hz = pwm_init(PWM_DEV(i), MODE, FREQU, STEPS);
        if (pwm_Hz == 0) {
            printf("Error initializing PWM_%i\n", i);
            return 1;
        }
        else {
            printf("Initialized PWM_%i @ %" PRIu32 "Hz\n", i, pwm_Hz); 
        }
    }
    /* for the thread running the shell */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    gcoap_fan_control_init();
    puts("All up, running the shell now");  // start shell command
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}
