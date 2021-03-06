#include "ClientServerReal.h"

bool ClientServerReal::Start(Mode mode, unsigned int port, const char * ip) {
  if (Mode::Client == mode
      && !ip) {
    return false;
  } /* ip may be null for server when binding to all interfaces */
  m_mode = mode;
  if (Mode::Server == mode) {
    return StartServer(port, ip);
  } else if (Mode::Client == mode) {
    return StartClient(port, ip);
  } else {
    return false;
  }
}

bool ClientServerReal::StartServer(unsigned int port, const char * ip) {

  if(!InitServer(port, ip)) {
    return false;
  }

  bool running = true;
  Packet* recv_buf =
    static_cast<Packet*>(m_packet_buffer.Allocate(MAX_PACKET_LENGTH));
  Packet* send_buf =
    static_cast<Packet*>(m_packet_buffer.Allocate(MAX_PACKET_LENGTH));


  char print_buf[128] = {0};
  Printf(print_buf,  print_buf + sizeof(print_buf),  "Server running\n");
  write(STDOUT, print_buf, strlen(print_buf));

  while (running) {
    memset(recv_buf, 0, MAX_PACKET_LENGTH);
    memset(send_buf, 0, MAX_PACKET_LENGTH);
    /* blocking */
    if (!RecvPacket(recv_buf)) {
      continue;
    }

    DebugPacket(recv_buf);
    if (!ServerCreateResponse(send_buf, recv_buf)) {

      continue;
    }

    DebugPacket(send_buf);
    if (!SendPacket(send_buf)) {

      continue;
    }
    if (Command::Shutdown == recv_buf->command) {

      if (!ShutdownServer()) {

        return false; /* actually return here */
      }
      running = false;
    }
  }

  m_packet_buffer.Deallocate(recv_buf);
  m_packet_buffer.Deallocate(send_buf);

  Printf(print_buf, print_buf + sizeof(print_buf),  "Server done\n");
  write(STDOUT, print_buf, strlen(print_buf));

  return true;
}

bool ClientServerReal::StartClient(unsigned int port, const char * ip) {


  if(!InitClient(port, ip)) {

    return false;
  }

  Packet* recv_buf =
    static_cast<Packet*>(m_packet_buffer.Allocate(MAX_PACKET_LENGTH));
  Packet* send_buf =
    static_cast<Packet*>(m_packet_buffer.Allocate(MAX_PACKET_LENGTH));
  memset(recv_buf, 0, MAX_PACKET_LENGTH);
  memset(send_buf, 0, MAX_PACKET_LENGTH);

  bool running = true;


  char print_buf[128] = {0};
  Printf(print_buf, print_buf + sizeof(print_buf), "Client running\n");
  write(STDOUT, print_buf, strlen(print_buf));

  /* arbitrary test commands */

  memset(send_buf, 0, MAX_PACKET_LENGTH);
  memset(recv_buf, 0, MAX_PACKET_LENGTH);
  char data[] = "TEST";
  CreatePacket(send_buf, sizeof(data), NextSequenceNumber(), Command::Echo, 11, data);

  DebugPacket(send_buf);
  SendPacket(send_buf);
  RecvPacket(recv_buf);

  DebugPacket(recv_buf);

  memset(send_buf, 0, MAX_PACKET_LENGTH);
  memset(recv_buf, 0, MAX_PACKET_LENGTH);
  CreatePacket(send_buf, 0, NextSequenceNumber(), Command::Invalid, 22, nullptr);

  DebugPacket(send_buf);
  SendPacket(send_buf);
  RecvPacket(recv_buf);

  DebugPacket(recv_buf);

  memset(send_buf, 0, MAX_PACKET_LENGTH);
  memset(recv_buf, 0, MAX_PACKET_LENGTH);
  CreatePacket(send_buf, 0, NextSequenceNumber(), Command::Unsupported, 33, nullptr);

  DebugPacket(send_buf);
  SendPacket(send_buf);
  RecvPacket(recv_buf);

  DebugPacket(recv_buf);

  /* ending with server shutdown */
  uint16_t handle = 66;
  while (running) {
    memset(send_buf, 0, MAX_PACKET_LENGTH);
    memset(recv_buf, 0, MAX_PACKET_LENGTH);
    CreatePacket(send_buf, 0, NextSequenceNumber(), Command::Shutdown, handle, nullptr);

    DebugPacket(send_buf);
    SendPacket(send_buf);
    RecvPacket(recv_buf);

    DebugPacket(recv_buf);
    if (send_buf->handle == recv_buf->handle
        && static_cast<uint16_t>(Command::ShutdownReply) == recv_buf->command) {

      running = false;
    } else {

    }
    handle++;
  }


  if(!ShutdownClient()) {

    return false;
  }

  m_packet_buffer.Deallocate(recv_buf);
  m_packet_buffer.Deallocate(send_buf);

  Printf(print_buf,  print_buf + sizeof(print_buf), "Client done\n");
  write(STDOUT, print_buf, strlen(print_buf));

  return true;
}

bool ClientServerReal::InitServer(unsigned int port, const char * ip) {
  /* ip may be null for INADDR_ANY */

  if(!port) {
    return false;
  }
  /* create socket */
  m_socket = socket (AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);
  if (m_socket < 0) {

    return false;
  }
  /* Bind server to given port and ip or any ip if nullprt */
  /* we dont need other_addr here */
  /* step 1 find out the server address we are on */
  memset((char*) &m_my_addr, 0, sizeof(m_my_addr));
  m_my_addr.sin_family = AF_INET;
  m_my_addr.sin_port = htons(port);
  if (ip) {
    hostent *hp; /* host information */
    hp = gethostbyname(ip);
    if (hp) {
      memcpy((void*) &(m_my_addr.sin_addr), hp->h_addr_list[0], hp->h_length);
      /* just pick the first one here, TODO validate that its ipv4 */
    } else {

      m_my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
  } else {
    m_my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  }

  /* step 2 do the bind to server address */
  if (bind(m_socket, reinterpret_cast<sockaddr*>(&m_my_addr), sizeof(m_my_addr)) < 0) {

    return false;
  }

  /* all went well */
  return true;
}

bool ClientServerReal::InitClient(unsigned int port, const char * ip) {

  if (!ip || !port) {
    return false;
  }
  /* create socket */
  m_socket = socket (AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);
  if (m_socket < 0) {

    return false;
  }

  /* find our address to be any, this is where we bind the socket to */
  memset((char*) &m_my_addr, 0, sizeof(m_my_addr));
  m_my_addr.sin_family = AF_INET;
  m_my_addr.sin_port = htons(0); /* any port */
  m_my_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* any address */

  /* find out server address, this is where we send messages */
  memset((char*) &m_other_addr, 0, sizeof(m_other_addr));
  m_other_addr.sin_family = AF_INET;
  m_other_addr.sin_port = htons(port);
  hostent *hp; /* host information */
  hp = gethostbyname(ip);
  if (hp) {
    memcpy((void*) &(m_other_addr.sin_addr), hp->h_addr_list[0], hp->h_length);
    /* just pick the first one here, TODO validate that its ipv4 */
  } else {

    return false;
  }

  /* bind to client addr */
  if (bind(m_socket, reinterpret_cast<sockaddr*>(&m_my_addr), sizeof(m_my_addr)) < 0) {

    return false;
  }

  /* all went fine */
  return true;
}

bool ClientServerReal::ShutdownServer() {

  close(m_socket);
  return true;
}

bool ClientServerReal::ShutdownClient() {

  close(m_socket);
  return true;
}

bool ClientServerReal::ServerCreateResponse(Packet* buffer, Packet* recieved) {
  if(!buffer
      || !recieved) {
    return false;
  }
  switch (static_cast<Command>(recieved->command)) {
    case Command::Echo : {
      return CreatePacket(buffer, recieved->payloadLength,
                          NextSequenceNumber(), Command::EchoReply,
                          recieved->handle, recieved->data);
    } break;
    case Command::Shutdown : {
      return CreatePacket(buffer, 0, NextSequenceNumber(),
                          Command::ShutdownReply, recieved->handle, nullptr);
    } break;
    default: {
      uint16_t data[1] = {recieved->command};
      return CreatePacket(buffer, sizeof(data), NextSequenceNumber(),
                          Command::GeneralErrorReply, recieved->handle,
                          data);
    } break;
  }
}

bool ClientServerReal::CreatePacket(Packet* buffer, uint16_t payloadLength,
                  uint16_t sequenceNumber, Command command,
                  uint16_t handle, const void* data_buf) {
  if (!buffer
      || payloadLength > MAX_PAYLOAD_LENGTH) {
    return false;
  }
  buffer->payloadLength = payloadLength;
  buffer->sequenceNumber = sequenceNumber;
  buffer->command = static_cast<uint16_t>(command);
  buffer->handle = handle;
  if(data_buf) {
    std::memcpy(buffer->data, data_buf, payloadLength);
  }
  return true;
}

bool ClientServerReal::SendPacket(const Packet* packet) {
  if (!packet) {
    return false;
  }
  uint8_t buf[MAX_TRANSMISSION_LENGTH];
  Wrapper* recv_buf = reinterpret_cast<Wrapper*>(buf);
  for (size_t retries = 0; retries < MAX_SEND_RETRIES; retries++) {
    uint16_t send_seq = NextWrapperNumber();
    if (!SendDataWrapper(packet, send_seq)) {

      //usleep(10);
      continue;
    }
    if (!RecvWrapper(recv_buf, send_seq)
        || static_cast<uint16_t>(WrapperType::ACK) != recv_buf->type) {

      //usleep(10);
      continue;
    }
    /* went well */
    return true;
  }
  /* looped to long, failure */
  return false;
}

/* blocking */
bool ClientServerReal::RecvPacket(Packet* buffer) {
  if (!buffer) {
    return false;
  }
  uint8_t buf[MAX_TRANSMISSION_LENGTH];
  Wrapper* recv_buf = reinterpret_cast<Wrapper*>(buf);
  if (!RecvWrapper(recv_buf, 0)
      || static_cast<uint16_t>(WrapperType::DATA) != recv_buf->type) {
    if (!SendNackWrapper(recv_buf->sequenceNumber, NextWrapperNumber())) {
      return false;
    }
  } else {
    if (!SendAckWrapper(recv_buf->sequenceNumber, NextWrapperNumber())) {
      return false;
    }
  }
  memcpy(buffer, recv_buf->packet, recv_buf->packet->payloadLength + sizeof(Packet));
  /* all went well */
  return true;
}

/* will not convert payload */
void ClientServerReal::HostToNetPacket(Packet* packet) {
  if (!packet) {
    return;
  }
  /* htons converts host to net uint16_t */
  packet->payloadLength   = htons(packet->payloadLength);
  packet->sequenceNumber  = htons(packet->sequenceNumber);
  packet->command         = htons(packet->command);
  packet->handle          = htons(packet->handle);
}

/* will not convert payload */
void ClientServerReal::NetToHostPacket(Packet* packet) {
  if (!packet) {
    return;
  }
  /* htons converts net to host uint16_t */
  packet->payloadLength   = ntohs(packet->payloadLength);
  packet->sequenceNumber  = ntohs(packet->sequenceNumber);
  packet->command         = ntohs(packet->command);
  packet->handle          = ntohs(packet->handle);
}

uint16_t ClientServerReal::NextSequenceNumber() {
  return m_sequence_number++;
}

void ClientServerReal::DebugPacket(const Packet* packet) {
  if (!packet) {
    return;
  }
}

uint16_t ClientServerReal::PacketChecksum(const Packet* packet) {
  if (!packet) {
    return 0;
  }
  return fletcher_16(reinterpret_cast<const uint8_t*>(packet),
                      sizeof(Packet) + packet->payloadLength);
}

bool ClientServerReal::ValidateWrapper(const Wrapper* wrapper, ssize_t bytes_recieved,
                                    uint16_t ack_expected_number) {

  DebugWrapper(wrapper);
  if (!wrapper) {
    return false;
  }
  if (bytes_recieved < static_cast<ssize_t>(sizeof(Wrapper))) {

    return false;
  }
  if (wrapper->totalLength != bytes_recieved) {

    return false;
  }
  switch (wrapper->type) {
    case static_cast<WrapperType>(WrapperType::ACK): /* fallthrough */
    case static_cast<WrapperType>(WrapperType::NACK): {

      if (wrapper->totalLength != sizeof(Wrapper)) {

        return false;
      }
      if (wrapper->ackNumber != ack_expected_number) {

        return false;
      }
      if (wrapper->packetChecksum != 0) {

        return false;
      }
    } break;
    case static_cast<WrapperType>(WrapperType::DATA): {

      if (wrapper->totalLength !=
            sizeof(Wrapper) + sizeof(Packet) + wrapper->packet->payloadLength) {

        return false;
      }
      if (wrapper->ackNumber != 0) {

        return false;
      }
      if (wrapper->packetChecksum != PacketChecksum(wrapper->packet)) {

        return false;
      }
    } break;
    default: {

      return false;
    }
  }

  return true;
}

bool ClientServerReal::SendAckWrapper(uint16_t recieved_seq, uint16_t seq_number) {
  Wrapper wrapper;
  wrapper.totalLength     = sizeof(Wrapper);
  wrapper.sequenceNumber  = seq_number;
  wrapper.ackNumber       = recieved_seq;
  wrapper.packetChecksum  = 0;
  wrapper.type            = static_cast<uint16_t>(WrapperType::ACK);
  auto size = wrapper.totalLength;

  DebugWrapper(&wrapper);
  HostToNetWrapper(&wrapper);
  if(sendto(m_socket, &wrapper, size, 0,
     reinterpret_cast<sockaddr*>(&m_other_addr), sizeof(m_other_addr)) < 0) {

    return false;
  }
  return true;
}

bool ClientServerReal::SendNackWrapper(uint16_t recieved_seq, uint16_t seq_number) {
  Wrapper wrapper;
  wrapper.totalLength     = sizeof(Wrapper);
  wrapper.sequenceNumber  = seq_number;
  wrapper.ackNumber       = recieved_seq;
  wrapper.packetChecksum        = 0;
  wrapper.type            = static_cast<uint16_t>(WrapperType::NACK);
  auto size = wrapper.totalLength;

  DebugWrapper(&wrapper);
  HostToNetWrapper(&wrapper);
  if(sendto(m_socket, &wrapper, size, 0,
     reinterpret_cast<sockaddr*>(&m_other_addr), sizeof(m_other_addr)) < 0) {

    return false;
  }
  return true;
}

bool ClientServerReal::SendDataWrapper(const Packet* packet, uint16_t seq_number) {
  if(!packet) {
    return false;
  }
  uint8_t buffer[MAX_TRANSMISSION_LENGTH] = {0};
  Wrapper* wrapper        = reinterpret_cast<Wrapper*>(buffer);
  wrapper->totalLength    = sizeof(Wrapper) + sizeof(Packet) + packet->payloadLength;
  wrapper->sequenceNumber = seq_number;
  wrapper->ackNumber      = 0;
  wrapper->packetChecksum = PacketChecksum(packet);
  wrapper->type           = static_cast<uint16_t>(WrapperType::DATA);
  memcpy(wrapper->packet, packet, sizeof(Packet) + packet->payloadLength);
  auto size = wrapper->totalLength;

  DebugWrapper(wrapper);
  HostToNetPacket(wrapper->packet);
  HostToNetWrapper(wrapper);
  if(sendto(m_socket, wrapper, size, 0,
     reinterpret_cast<sockaddr*>(&m_other_addr), sizeof(m_other_addr)) < 0) {

    return false;
  }
  return true;
}

/* blocking and validating */
bool ClientServerReal::RecvWrapper(Wrapper* buffer, uint16_t ack_expected_number) {
  if(!buffer) {
    return false;
  }
  sockaddr_in recv_addr;
  socklen_t recv_addr_len = sizeof(recv_addr);
  auto recieved_bytes = recvfrom(m_socket, buffer, MAX_TRANSMISSION_LENGTH, 0,
                                  reinterpret_cast<sockaddr*>(&recv_addr),
                                  &recv_addr_len);
  NetToHostWrapper(buffer);
  if (static_cast<uint16_t>(WrapperType::DATA) == buffer->type) {

    NetToHostPacket(buffer->packet);
  }

  DebugWrapper(buffer);
  if (Mode::Server == m_mode) {
    /* hopefully we only talk to one person */
    memcpy(&m_other_addr, &recv_addr, sizeof(recv_addr));
  }
  return ValidateWrapper(buffer, recieved_bytes, ack_expected_number);
}

/* will not convert possible packet */
void ClientServerReal::HostToNetWrapper(Wrapper* wrapper) {
  if (!wrapper) {
    return;
  }
  wrapper->totalLength    = htons(wrapper->totalLength);
  wrapper->sequenceNumber = htons(wrapper->sequenceNumber);
  wrapper->ackNumber      = htons(wrapper->ackNumber);
  wrapper->packetChecksum = htons(wrapper->packetChecksum);
  /* type must not be reordered */
}

/* will not convert possible packet */
void ClientServerReal::NetToHostWrapper(Wrapper* wrapper) {
  if (!wrapper) {
    return;
  }
  wrapper->totalLength    = ntohs(wrapper->totalLength);
  wrapper->sequenceNumber = ntohs(wrapper->sequenceNumber);
  wrapper->ackNumber      = ntohs(wrapper->ackNumber);
  wrapper->packetChecksum = ntohs(wrapper->packetChecksum);
  /* type must not be reordered */
}

uint16_t ClientServerReal::NextWrapperNumber() {
  return m_wrapper_number++;
}

void ClientServerReal::DebugWrapper(const Wrapper* wrapper) {
  if(!wrapper) {
    return;
  }
  if (wrapper->type == static_cast<uint16_t>(WrapperType::DATA)) {

    DebugPacket(wrapper->packet);
  } else {

  }
}

uint16_t fletcher_16 (const uint8_t* data, const size_t length) {
  uint16_t sum1 = 0, sum2 = 0;
  for (size_t i = 0; i < length; i++) {
    sum1 = (sum1 + data[i]) % 255;
    sum2 = (sum2 + sum1) % 255;
  }
  return (sum2 << 8) | sum1;
}
