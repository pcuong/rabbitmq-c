#include <amqp.h>

int main(int argc, const char* argv[])
{
  amqp_connection_state_t conn;
  int sockfd;

  conn = amqp_new_connection();
  sockfd = amqp_open_socket("localhost", AMQP_PROTOCOL_PORT);

  amqp_set_sockfd(conn, sockfd);

  amqp_login(conn, "/", AMQP_DEFAULT_MAX_CHANNELS, AMQP_DEFAULT_FRAME_SIZE, AMQP_DEFAULT_HEARTBEAT, AMQP_SASL_METHOD_PLAIN, "guest", "guest");

  amqp_connection_close(conn, AMQP_REPLY_TYPE_SUCCESS);
  amqp_destroy_connection(conn);
}
