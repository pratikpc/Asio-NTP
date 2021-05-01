#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <asio/signal_set.hpp>

#include <Client.hxx>

#include <iostream>
#include <thread>

using asio::awaitable;
using asio::detached;
using asio::use_awaitable;

namespace this_coro = asio::this_coro;

asio::awaitable<void> Run()
{
   // Change URL to point to your favourite NTP server
   auto connection = co_await pc::ntp::Connection::make_connection("in.pool.ntp.org");
   // Get Time as Unix Epoch
   std::time_t time_s = co_await connection.TimeFromUnixEpoch();
   std::cout << "\nTime is " << time_s;

   // Stringify
   std::tm* ptm = std::localtime(&time_s);
   char     buffer[32];
   // Format: Mo, 15.06.2009 20:20:00
   std::strftime(buffer, 32, "%a, %d.%m.%Y %H:%M:%S", ptm);
   std::cout << "\nStringified time is " << buffer;
}

int main()
{
   try
   {
      static auto constexpr THREAD_COUNT = 2;
      asio::io_context                          io_context(THREAD_COUNT);
      std::array<std::thread, THREAD_COUNT - 1> threads;

      asio::signal_set signals(io_context, SIGINT, SIGTERM);
      signals.async_wait([&io_context](auto, auto) {
         std::cout << "Stopping server\n";
         io_context.stop();
      });

      for (auto& thread : threads)
         thread = std::thread([&io_context] {
            std::cout << std::this_thread::get_id() << " started\n";
            io_context.run();
            std::cout << std::this_thread::get_id() << " over\n";
         });

      std::cout << std::this_thread::get_id() << " started\n";

      asio::co_spawn(io_context, Run(), asio::detached);

      // Run the I/O service on the requested number of threads
      io_context.run();

      std::cout << std::this_thread::get_id() << " overed\n";
      for (auto& thread : threads)
         if (thread.joinable())
            thread.join();
   }
   catch (...)
   {
      std::cout << "Exception";
   }
   return EXIT_SUCCESS;
}
