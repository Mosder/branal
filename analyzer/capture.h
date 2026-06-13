// capture.h
// functions for capturing packets using tshark

// starts capturing mode
// params:
//      - path - file path to capture the .pcap file to,
//               if NULL, saves it to default capture directory as capture-<date>_<time>.pcap
extern void capturing_mode(char *path);
