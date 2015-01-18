CXX=g++
CXXFLAGS =	-O3 -g -Wall -fmessage-length=0

SRCS = Segment.cpp Chrom.cpp ChromPair.cpp Population.cpp GeneralModel.cpp AdmSimulator.cpp generator.cpp

OBJS = $(SRCS:.cpp=.o)

LIBS =

TARGET = AdmSimulator

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET) reader AdmixModel

reader:	reader.cpp
	$(CXX) $(CXXFLAGS) -o reader reader.cpp

AdmixModel: Segment.o Chrom.o ChromPair.o Population.o Model.cpp AdmixModel.cpp generator.o

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) reader reader.o Model.o AdmixModel.o AdmixModel
