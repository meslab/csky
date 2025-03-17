#include "../../crux/include/logger.h"
#include "../include/ads_b.h"
#include "../include/arg_parser.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
	Options opts;
	arguments_parse(argc, argv, &opts);

	LoggerOptions logger_options = {
		.verbose = opts.verbose,
		.log_level = opts.log_level,
		.err_log = opts.err_log,
		.out_log = opts.out_log	
	};

	Logger logger;
	logger_init(&logger, &logger_options);

	// Example 26-character hex string (13 bytes)
	const char *hex_str_26 = "7890ABCDEF1234567890ABCDEF";
	printf("Processing 26-char hex string:\n");
	adsb_squitter_parse(&logger, hex_str_26);

	// Example 40-character hex string (20 bytes)
	const char *hex_str_40 = "1234567890ABCDEF1234567890ABCDEF12345678";
	printf("\nProcessing 40-char hex string:\n");
	adsb_squitter_parse(&logger, hex_str_40);

	return 0;
}
