//! @file mock_client.h
//! @author Jennifer Gott
//! @date 2025-05-06
//! @brief Mock client for testing
//! @note This is a mock client for testing
//! @warning Do not use this in production code
//! @warning This is not a real client, it is a mock

#ifndef MOCK_CLIENT_H
#define MOCK_CLIENT_H

#ifdef UNIT_TEST

#include <cstdint>
#include <string>

class Client {
 private:
  std::string m_server_name;
  uint16_t m_port;
  std::string m_mock_data;
  size_t m_read_pos {0};
  bool m_connected {false};
  bool m_next_connect_result {true};
  bool m_next_write_result {true};
  size_t m_next_write_result_size {0};
  bool m_next_read_result {true};
  size_t m_next_read_result_size {0};
  bool m_next_available_result {true};
  size_t m_next_available_result_size {0};

 public:
  //! @brief Constructor
  Client() = default;

  //! @brief Connect to the server
  //! @param host The server name
  //! @param port The port number
  //! @return 1 if the connection is successful, 0 otherwise
  int connect(const char* host, uint16_t port) { 
    m_server_name = host;
    m_port = port;
    m_connected = m_next_connect_result;
    return m_connected ? 1 : 0;
  }
  
  //! @brief Stop the client
  //! @note This is a no-op for the mock client
  void stop() {}

  //! @brief Check if there is data available to read
  //! @return The number of bytes available to read
  int available() { 
    if (!m_connected) 
    {
      return 0;
    }
    return m_mock_data.length() - m_read_pos; 
  }

  //! @brief Read data from the client
  //! @return The next byte in the mock data or -1 if no more data is available
  int read() 
  {
    size_t available_bytes = m_mock_data.length() - m_read_pos;
    available_bytes = std::min(available_bytes, m_next_available_result_size);
    if (!m_connected || available_bytes == 0) 
    {
      return -1;
    }
    if (available_bytes > 0 && m_next_read_result) 
    {
      return m_mock_data[m_read_pos++];
    }
    return -1;
  }

  //! @brief Peeks at the next byte in the mock data
  //! @return The next byte in the mock data or -1 if no more data is available
  int peek() 
  {
    size_t available_bytes = m_mock_data.length() - m_read_pos;
    available_bytes = std::min(available_bytes, m_next_available_result_size);
    if (!m_connected || available_bytes == 0 || !m_next_available_result) 
    {
      return -1;
    }
    return m_mock_data[m_read_pos];
  }

  //! @brief Write data to the client
  //! @param buf The data to write
  //! @param size The size of the data to write
  //! @return The size of the data written
  size_t write(const uint8_t* buf, size_t size) { 
    if (!m_connected || !m_next_write_result) 
    {
      return 0;
    }
    m_mock_data.append(reinterpret_cast<const char*>(buf), size);
    return size; 
  }


  //! @brief Check if the client is connected
  //! @return true if the client is connected, false otherwise
  operator bool() { return m_connected; }

  //! @brief Set the mock response
  //! @param data The mock response
  void set_mock_response(const std::string& data) {
    m_mock_data = data;
    m_read_pos = 0;
  }

  //! @brief Set the server name
  //! @param server_name The server details
  void set_server_name(const std::string& server_name, uint16_t port) { 
    m_server_name = server_name; 
    m_port = port; 
  }

  //! @brief Set the next connect result
  //! @param result The next connect result
  void set_next_connect_result(bool result) { m_next_connect_result = result; }

  //! @brief Set the connection status
  //! @param connected The connection status
  void set_connected(bool connected) { m_connected = connected; }

  //! @brief Set the next write result
  //! @param result The next write result
  void set_next_write_result(bool result) { m_next_write_result = result; }

  //! @brief Set the next read result
  //! @param result The next read result
  void set_next_read_result(bool result) { m_next_read_result = result; }

  //! @brief Set the next available result
  //! @param result The next available result
  void set_next_available_result(bool result) { m_next_available_result = result; }

  //! @brief Set the next available result size
  //! @param size The next available result size
  void set_next_available_result_size(size_t size) { m_next_available_result_size = size; }

  //! @brief Set the next write result size
  //! @param size The next write result size
  void set_next_write_result_size(size_t size) { m_next_write_result_size = size; }
  
};

#endif  // UNIT_TEST

#endif  // MOCK_CLIENT_H