#include "cmn.h"
#include "virtual_socketcan.h"
#include <errno.h>
#include <error.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int g_sockfd;                  // CAN socket FD
bool port_should_exit = false; // flag for shutting down

/**
 * @brief poll for CAN messages
 * @return 0 if message is present, -1 otherwise
 */
enum Iso14229CANRxStatus portCANRxPoll(uint32_t *arb_id, uint8_t *data, uint8_t *size) {
    struct can_frame frame = {0};
    // printf("portCANRxPoll reading\n");
    int nbytes = read(g_sockfd, &frame, sizeof(struct can_frame));
    if (nbytes < 0) {
        if (EAGAIN == errno || EWOULDBLOCK == errno) {
            // printf("portCANRxPoll kCANRxNone\n");
            return kCANRxNone;
        } else {
            perror("Read err");
            exit(-1);
        }
    }
    *arb_id = frame.can_id;
    *size = frame.can_dlc;
    memmove(data, frame.data, *size);
    printf("portRecvCAN send> 0x%03x: size:%d ", *arb_id, *size);
    PRINTHEX(frame.data, *size);
    return kCANRxSome;
}

struct sigaction action;
struct sockaddr_can addr;
struct ifreq ifr;
struct stat fd_stat;
FILE *fd;

/**
 * @brief close file descriptor on SIGINT
 * @param signum
 */
void teardown(int signum) {
    (void)signum;
    if (close(g_sockfd) < 0) {
        perror("failed to close socket");
        exit(-1);
    }
    port_should_exit = true;
}

/**
 * @brief can port send data
 * 
 * @param arbitration_id 
 * @param data 
 * @param size 
 * @return int 
 */
int portSendCAN(const uint32_t arbitration_id, const uint8_t *data, const uint8_t size) {
    struct can_frame frame = {0};

    frame.can_id = arbitration_id;
    frame.can_dlc = size;
    memmove(frame.data, data, size);

    if (write(g_sockfd, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
        perror("Write err");
        exit(-1);
    }
    // printf("send> 0x%03x: ", arbitration_id);
    printf("portSendCAN send> 0x%03x: size:%d ", arbitration_id, size);

    PRINTHEX(data, size);
    return 0;
}

/**
 * @brief can port init
 * 
 * @param ac 
 * @param av 
 */
int portSetup(char *dev) {
    printf("port portSetup init dev:%s \n", dev);

    memset(&action, 0, sizeof(action));
    action.sa_handler = teardown;
    sigaction(SIGINT, &action, NULL);

    if (dev == NULL)
    {
        printf("dev is NULL\n");
        return -1;
    }

    if ((g_sockfd = socket(PF_CAN, SOCK_RAW | SOCK_NONBLOCK, CAN_RAW)) < 0) {
        perror("Socket");
        return -1;
    }

    strcpy(ifr.ifr_name, dev);
    ioctl(g_sockfd, SIOCGIFINDEX, &ifr);

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(g_sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind");
        return -1;
    }

    // Try sending a message. This will fail if the network is down.
    portSendCAN(0x111, (uint8_t[4]){1, 2, 3, 4}, 4);

    printf("listening on %s\n", dev);

    return 0;
}

/**
 * @brief get local time milliseconds
 * 
 * @return uint32_t 
 */
uint32_t portGetms() {
    struct timeval te;
    gettimeofday(&te, NULL);                                         // get current time
    long long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000; // calculate milliseconds
    return milliseconds;
}

/**
 * @brief sleep tms milliseconds
 * 
 * @param tms 
 */
void portYieldms(uint32_t tms) {
    struct timespec ts;
    int ret;

    ts.tv_sec = tms / 1000;
    ts.tv_nsec = (tms % 1000) * 1000000;

    do {
        ret = nanosleep(&ts, &ts);
    } while (ret && errno == EINTR);

    // return ret;
}

/**
 * @brief 
 * 
 * @param fmt 
 * @param ... 
 */
void isotp_user_debug(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}