#ifndef MESSAGE_TRANSFER_H
#define MESSAGE_TRANSFER_H

#include "message.h"

void send_message(int socket_fd, const Message& message);

Message recv_message(int socket_fd);

#endif