#
# c++11
#
QMAKE_CXXFLAGS += -std=c++11

#
# folly
#
INCLUDEPATH += $${PWD}
LIBS += -L$${PWD}/folly/.libs
LIBS += -lfolly
LIBS += -lfollybenchmark

#
# glog
#
LIBS += -lglog

#
# remove warning
#
# unused-variable
QMAKE_CXXFLAGS += -Wno-unused-parameter

