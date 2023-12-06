#include "tcp_protocol.h"
#include <pthread.h>
#include <stdbool.h>
#include <sys/queue.h>
#include <time.h>

#ifndef TCP_OP_H
#define TCP_OP_H

#define DEFAULT_RTO 3

extern uint32_t SEQNUM;
extern uint16_t CWND;
extern uint32_t sent_size;

struct tcp_packet_entry
{
  TAILQ_ENTRY (tcp_packet_entry) entry;
  tcp_hdr_t *hdr;
  bool checked;
};
typedef struct tcp_packet_entry tcp_packet_entry_t;

struct tcp_check_entry
{
  TAILQ_ENTRY (tcp_check_entry) entry;
  tcp_hdr_t *hdr;
  time_t timeout;
  bool checked;
} __attribute__ ((packed));
typedef struct tcp_check_entry tcp_check_entry_t;

TAILQ_HEAD (tcp_iq, tcp_packet_entry);
struct tcp_iq tcp_inq;

TAILQ_HEAD (tcp_cq, tcp_check_entry);
struct tcp_cq tcp_ckq;

pthread_mutex_t inq_lock;
pthread_cond_t inq_cond;

void *tcp_check_timeout ();
void handle_tcp (tcp_hdr_t *hdr);
tcp_hdr_t *tcp_wait_packet (uint32_t target_ack, time_t timeout, uint8_t flag);
void tcp_add_sw_packet (uint32_t target_ack, time_t timeout);

uint32_t tcp_handshake (int socket, in_addr_t src_ip, struct sockaddr_in sin);
uint32_t tcp_stop_and_wait (int socket, in_addr_t src_ip,
                            struct sockaddr_in sin, uint32_t ack_num,
                            uint32_t num_byte);
uint32_t tcp_send_sliding_window_fixed (int socket, in_addr_t src_ip,
                                        struct sockaddr_in sin,
                                        uint32_t ack_num, uint32_t num_byte);
void tcp_teardown (int socket, in_addr_t src_ip, struct sockaddr_in sin,
                   uint32_t ack_num);

#endif /* -- TCP_OP_H -- */