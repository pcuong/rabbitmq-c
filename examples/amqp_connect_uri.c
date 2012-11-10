#include <amqp.h>
#include <stdio.h>

int main(int argc, const char* argv[])
{
  amqp_connection_state_t conn;
  int sockfd;
  struct amqp_connection_info info;

  if (argc != 2)
  {
    printf("Usage: %s URI\n", argv[0]);
    return 1;
  }

  amqp_default_connection_info(&info);

  if (!amqp_parse_url(argv[1], &info))
  {
    printf("%s: The AMQP URI '%s' is invalid.\n", argv[0], argv[1]);
    return 2;
  }

  conn = amqp_new_connection();
  sockfd = amqp_open_socket(info.host, info.port);

  amqp_set_sockfd(conn, sockfd);

  amqp_login(conn, info.vhost, AMQP_DEFAULT_MAX_CHANNELS, AMQP_DEFAULT_FRAME_SIZE, AMQP_DEFAULT_HEARTBEAT, AMQP_SASL_METHOD_PLAIN, info.user, info.password);

  amqp_connection_close(conn, AMQP_REPLY_TYPE_SUCCESS);
  amqp_destroy_connection(conn);
}

