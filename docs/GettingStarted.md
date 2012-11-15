# Getting started with rabbitmq-c

[TOC]

## Connecting to the broker
The procedure for connecting to the broker is to open a socket to the broker
on the desired port, then perform the handshake with the broker to authenticate
with the broker, select a virtual host (vhost) and set various parameters.

The first step is to create a new `#amqp_connection_state_t` object.  This object
represents a single connection to the broker and is passed into most rabbitmq-c
API functions.  The `#amqp_connection_state_t` object is created by:

~~~{.c}
amqp_connection_state_t conn = amqp_new_connection();
~~~

Once a connection object is created, a socket must be opened to the broker. A
platform-independant wrapper function `#amqp_open_socket` can be used to open
the socket. This socket should then be set in the connection object using the
`#amqp_set_sockfd` function:

~~~{.c}
int sockfd = amqp_open_socket("localhost", AMQP_PROTOCOL_PORT);
amqp_set_sockfd(conn, sockfd);
~~~

Once the socket to the broker is open, one must authenticate with the broker:

~~~{.c}
amqp_login(conn, "/", 
    AMQP_DEFAULT_MAX_CHANNELS,
    AMQP_DEFAULT_FRAME_SIZE,
    AMQP_DEFAULT_HEARTBEAT,
    AMQP_SASL_METHOD_PLAIN, "guest", "guest");
~~~

The connection is then open can can be used. Once the connection is no longer
needed it can be disconnected from the broker and destroyed by:

~~~{.c}
amqp_connection_close(conn, AMQP_REPLY_TYPE_SUCCESS);
amqp_destroy_connection(conn);
~~~

Putting all of that together:

@include amqp_connect.c

## Parsing an AMQP URL

The AMQP connection parameters can be specified in a URL style format such
as: `amqp://username:password@hostname:5672/vhost`

rabbitmq-c provides a method to parse these URLs `#amqp_parse_url`

~~~{.c}
struct amqp_connection_info info;
amqp_default_connection_info(&info);
amqp_parse_url("amqp://user:pass@host:5672//", &info);
~~~

Adding it to the previous example we get:

@include amqp_connect_uri.c

## Interacting with the broker

So far our examples have focused on connecting to the broker: now to actually
do something on the broker - lets declare a queue.

In order to do anything on the broker you must open a channel on the
connection. Opening a channel is easy, pick a channel number that is not 
already in use, then invoke the `#amqp_channel_open()` function.

~~~{.c}
amqp_channel_open(conn, 1);
~~~

Channels are identified by a `#amqp_channel_t`, which is nothing more than a
number between 1 and 65535, you can pick whichever you like. Opening a channel
twice, however will cause an error, so its a good idea to keep track of which
channels you have open.

Channels also provide an error scope. If an error occurs a channel-exception is
typically thrown, by the broker to the client. Once a channel-exception occurs
the channel is closed must be opened again before using it.

If something really bad happens, then a connection-exception is thrown. In this
case the whole connection is closed. Once this happens the connection must be
destroyed and recreated.  An example of what can cause a connection exception
is using a channel after it has been closed.

Anyhow, lets create a queue on the broker:

~~~{.c}
amqp_queue_declare(conn, 1, amqp_cstring_bytes("queue_name"), 
    0, /* passive = actually create the queue instead of testing for it */
    0, /* durable = false */
    1, /* exclusive = true */
    1, /* auto_delete = true */
    amqp_empty_table);
~~~
