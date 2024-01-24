JFLAGS = -Xlint:all
JC = javac
JVM = java
MAIN = TareaSO3
.PHONY: all run

JAVAS := $(wildcard *.java)
CLASSES := $(JAVAS:.java=.class)

all: $(CLASSES)

%.class: %.java
	$(JC) $(JFLAGS) $^

run: $(CLASSES)
	$(JVM) $(MAIN)