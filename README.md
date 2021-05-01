# Asio Network Time Protocol


## Sample Usage

```cpp
 // Change URL to point to your favourite NTP server
 auto connection = co_await pc::ntp::Connection::make_connection("in.pool.ntp.org");
 // Get Time as Unix Epoch
 std::uint32_t time_s = co_await connection.TimeFromUnixEpoch();
 std::cout << "Time is " << time_s;
```

It is that easy.

We are utilising [Asio Standalone](https://think-async.com/Asio/) and using the latest and greatest Coroutines to simplify the code.  
Without coroutines of any form, be it Boost.Coroutine or others, the code would be full of callbacks and would be almost unreadable,  
at least for mere mortals such as yours truly.

## Customization

### Send a packet yourself
```cpp     
 ntp::Packet sendPacket;
 // Set li = 0, vn = 3, and mode = 3
 sendPacket.li_vn_mode = 0b00100011;

 co_await connection.Packet(sendPacket);
```
Where connection is a connection object

### Receive a packet yourself
```cpp
  ntp::Packet const packet = connection.co_await Packet();
```

### Resolve sockets myself instead of using Helper Function
```cpp
  udp::resolver resolver{executor};
  endpoint_t    receiver_endpoint =
      *(co_await resolver.async_resolve(protocol, host, "ntp", use_awaitable));
```
Your friendly neighbourhood Asio code to resolve UDP sockets

```cpp
  ntp::Connection connection{executor,
                             receiver_endpoint.protocol(),
                             std::forward<endpoint_t>(receiver_endpoint)};
```
And your friendly neighhbourhood constructor

## Contact me
For further details, you can contact me [via LinkedIn](https://www.linkedin.com/in/pratik-chowdhury-889bb2183/)
