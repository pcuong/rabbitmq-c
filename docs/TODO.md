# rabbitmq-c developer TODOs:
 - Use pkg-config to find popt
 - CMake: Add option for both Shared and Static libraries
 - CMake: Deal with in-source build correctly
 - Release source distribution packages
 - create amqp_sockfd_t
 - Fix const correctness issue in amqp_parse_uri
 - amqp_connection_info should be a typedef
 - convenience function to connect to broker (instead of amqp_amqp_open_socket,
     amqp_set_sockfd, amqp_login)
  
