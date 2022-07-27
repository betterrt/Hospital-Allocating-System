all:    scheduler.cpp   hospitalA.cpp   hospitalB.cpp   hospitalC.cpp   client.cpp
	g++ scheduler.cpp -o scheduler
	g++ hospitalA.cpp -o hospitalA
	g++ hospitalB.cpp -o hospitalB
	g++ hospitalC.cpp -o hospitalC
	g++ client.cpp -o client

.PHONY: scheduler
scheduler:
	./scheduler

.PHONY: hospitalA
hospitalA:
	./hospitalA

.PHONY: hospitalB
hospitalB:
	./hospitalB

.PHONY: hospitalC
hospitalC:
	./hospitalC

.PHONY: client
client:
	./client
