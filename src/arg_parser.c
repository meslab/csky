#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "arg_parser.h"
#include "crux_utils.h"
/**
 * @brief Parse the command line arguments
 *
 * @param argc Number of arguments
 * @param argv Arguments
 * @param opts Options struct
 */
void arguments_parse(int argc, char *argv[], Options *opts) {

	opts->tcp_server = "172.22.132.124";
	opts->tcp_port = 30002;
	opts->verbose = 0;
	opts->err_log = NULL;
	opts->out_log = NULL;
	opts->log_level = "ERROR";

	static struct option long_options[] = {
	    {"tcp-server", optional_argument, 0, 't'},
	    {"tcp-port", optional_argument, 0, 'p'},
	    {"log", optional_argument, 0, 'o'},
	    {"error-log", optional_argument, 0, 'e'},
	    {"log-level", optional_argument, 0, 'l'},
	    {"verbose", no_argument, 0, 'v'},
	    {"help", no_argument, 0, 'h'},
	    {0, 0, 0, 0}};

	int opt;
	while ((opt = getopt_long(argc, argv, "t:p:o:e:l:vh", long_options,
				  NULL)) != -1) {
		switch (opt) {
		case 't':
			if (optarg) {
				opts->tcp_server = optarg;
			}
			break;
		case 'p':
			if (optarg) {
				opts->tcp_port = atoi(optarg);
			}
			break;
		case 'o':
			if (optarg) {
				opts->out_log = fopen(optarg, "a");
			}
			break;
		case 'e':
			if (optarg) {
				opts->err_log = fopen(optarg, "a");
			}
			break;
		case 'l':
			if (optarg) {
				opts->log_level = optarg;
				to_upper_case(opts->log_level);
			}
			break;
		case 'v':
			opts->verbose = 1;
			break;
		case 'h':
			printf("Usage: %s [-t | --tcp-server <ip.ad.dr.es>] "
			       "[-p | --tcp-port "
			       "value] [-o | --log value] [-e | --error-log "
			       "value] [-l | "
			       "--log-level value [-v | --verbose] [-h | "
			       "--help]\n",
			       argv[0]);
			exit(0);
		default:
			exit(1);
		}
	}
}
