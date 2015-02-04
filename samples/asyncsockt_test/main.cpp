#include <folly/io/async/AsyncSocket.h>
#include <folly/io/async/EventBase.h>
#include <glog/logging.h>

static void flush()
{
  google::FlushLogFiles(google::GLOG_INFO);
}

class MySocket :
  public folly::AsyncSocket,
  public folly::AsyncSocket::ConnectCallback,
  public folly::AsyncTransportWrapper::ReadCallback,
  public folly::AsyncTransportWrapper::WriteCallback
{
protected:
  static const int BUF_SIZE = 8192;
  unsigned char buffer[BUF_SIZE];

public:
  explicit MySocket(folly::EventBase* evb) : AsyncSocket(evb) {}

  //
  // override folly::AsyncSocket::ConnectCallback,
  //
  virtual void connectSuccess() noexcept
  {
    LOG(INFO) << "connectSuccess()"; flush();
    this->setReadCB(this);
    const char* request = "GET / HTTP/1.1\r\nHost: test.gilgil.net\r\n\r\n";
    this->write(this, request, strlen(request));
  }
  virtual void connectErr(const folly::AsyncSocketException& ex) noexcept
  {
    LOG(ERROR) << "connectErr() errno=" << ex.getErrno(); flush();
  }

  //
  // override folly::AsyncTransportWrapper::ReadCallback
  //
  virtual void getReadBuffer(void** bufReturn, size_t* lenReturn)
  {
    LOG(INFO) << "getReadBuffer() *bufReturn=" << *bufReturn << " *lenReturn=" << *lenReturn; flush();
    *bufReturn = buffer; *lenReturn = BUF_SIZE;
  }
  virtual void readDataAvailable(size_t len) noexcept
  {
    LOG(INFO) << "readDataAvailable() len=" << len; flush();
    LOG(INFO) << buffer << std::endl; flush();
  }
  virtual void readEOF() noexcept
  {
    LOG(INFO) << "readEOF()"; flush();
  }
  virtual void readErr(const folly::AsyncSocketException& ex) noexcept
  {
    LOG(ERROR) << "readErr() errno=" << ex.getErrno(); flush();
  }

  //
  // override folly::AsyncTransportWrapper::WriteCallback
  //
  virtual void writeSuccess() noexcept
  {
    LOG(INFO) << "writeSuccess()"; flush();
  }
  virtual void writeErr(size_t bytesWritten, const folly::AsyncSocketException& ex) noexcept
  {
    LOG(ERROR) << "writeErr() errno=" << ex.getErrno(); flush();
  }
};

void test()
{
  folly::EventBase evb;
  folly::SocketAddress addr("test.gilgil.net", 80, true);
  MySocket sock(&evb);
  sock.connect(&sock, addr);

  for (int i = 0; i < 30; i++)
  {
    evb.loop();
    usleep(100000);
  }
}

int main(int argc, char* argv[])
{
  (void)argc;
  (void)argv;
  google::InitGoogleLogging(argv[0]);

  LOG(INFO) << "beg"; flush();
  test();
  LOG(INFO) << "end"; flush();
  return 0;
}
