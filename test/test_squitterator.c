#include "../include/ads_b.h"
#include <stdio.h>

int main() {
  // Example 26-character hex string (13 bytes)
  const char *hex_str_26 = "7890ABCDEF1234567890ABCDEF";
  printf("Processing 26-char hex string:\n");
  process_adsb(hex_str_26);

  // Example 40-character hex string (20 bytes)
  const char *hex_str_40 = "1234567890ABCDEF1234567890ABCDEF12345678";
  printf("\nProcessing 40-char hex string:\n");
  process_adsb(hex_str_40);

  return 0;
}
