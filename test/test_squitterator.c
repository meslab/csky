#include "../include/ads_b.h"
#include "../include/logger.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  Options opts;
  parse_arguments(argc, argv, &opts);

  Logger logger;
  init_logger(&logger, &opts);

  // Example 26-character hex string (13 bytes)
  const char *hex_str_26 = "7890ABCDEF1234567890ABCDEF";
  printf("Processing 26-char hex string:\n");
  process_adsb(&logger, hex_str_26);

  // Example 40-character hex string (20 bytes)
  const char *hex_str_40 = "1234567890ABCDEF1234567890ABCDEF12345678";
  printf("\nProcessing 40-char hex string:\n");
  process_adsb(&logger, hex_str_40);

  return 0;
}
