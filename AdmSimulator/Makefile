CXX=g++
CXXFLAGS =	-O2 -g -Wall

SRCS = Segment.cpp Chrom.cpp ChromPair.cpp Population.cpp GeneralModel.cpp AncEvolve.cpp AdmSimulator.cpp

OBJS = $(SRCS:.cpp=.o)

LIBS = 

TARGET = AdmSimulator

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)