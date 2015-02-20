#ifndef DROPBEAR_NETIO_H
#define DROPBEAR_NETIO_H

#include "includes.h"
#include "buffer.h"
#include "queue.h"

enum dropbear_prio {
	DROPBEAR_PRIO_DEFAULT = 10,
	DROPBEAR_PRIO_LOWDELAY = 11,
	DROPBEAR_PRIO_BULK = 12,
};

void set_sock_nodelay(int sock);
void set_sock_priority(int sock, enum dropbear_prio prio);

void get_socket_address(int fd, char **local_host, char **local_port,
		char **remote_host, char **remote_port, int host_lookup);
void getaddrstring(struct sockaddr_storage* addr, 
		char **ret_host, char **ret_port, int host_lookup);
int dropbear_listen(const char* address, const char* port,
		int *socks, unsigned int sockcount, char **errstring, int *maxfd);

struct dropbear_progress_connection;

/* result is DROPBEAR_SUCCESS or DROPBEAR_FAILURE.
errstring is only set on DROPBEAR_FAILURE, returns failure message for the last attempted socket */
typedef void(*connect_callback)(int result, int sock, void* data, const char* errstring);

struct dropbear_progress_connection * connect_remote (const char* remotehost, const char* remoteport,
	connect_callback cb, void *cb_data);

void set_connect_fds(fd_set *writefd);
void handle_connect_fds(fd_set *writefd);

/* Doesn't actually stop the connect, but adds a dummy callback instead */
void cancel_connect(struct dropbear_progress_connection *c);

void connect_set_writequeue(struct dropbear_progress_connection *c, struct Queue *writequeue);

/* TODO: writev #ifdef guard */
struct iovec * packet_queue_to_iovec(struct Queue *queue, int *ret_iov_count);
void packet_queue_consume(struct Queue *queue, ssize_t written);

#endif

