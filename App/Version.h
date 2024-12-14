#pragma once

#define TO_STRING2(s) #s
#define TO_STRING(s) TO_STRING2(s)

#define MAJOR 7
#define MINOR 0
#define PATCH 1

#define VERSION_STR     "7.0.2"
#define VERSION			MAJOR, MINOR, PATCH

#define AP_VERSION_STR	"0.5.1"
#define AP_VERSION_STR_BACKCOMPAT  "0.4.5"

#define PRODUCT_NAME L"Witness Random Puzzle Generator for Archipelago.gg"
