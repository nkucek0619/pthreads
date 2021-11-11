# pthreads
## Operating Systems Lab 2: POSIX Threads and Semaphores
# Cigarette Ingredients
-Tobacco\
-Rolling Paper\
-Match
# Smokers
-3 chain smokers, 1 agent\
-Each smoker has only one ingredient with an infinite supply\
-An agent has an infinite supply of all 3 ingredients
# Making a Cigarette
-To make a cigarette, each smoker with their own ingredient must have ingredients from the other two smokers.\
-The agent and smokers share a table.\
-Agent randomly generates 2 ingredients and notifies the smoker who needs these 2 ingredients, and then the agent supplies another 2.\
-Each smoker waits for the agent's signal, then once signaled, the smoker picks up the ingredients, makes a cigarette, smokes for a while (wait), and goes back to the table waiting for his next ingredients.
# Problem
-Develop an algorithm for the smokers using semaphores as synchronization primitives.\
-Agent = operating system, smokers = applications that need resources\
-One or more applications proceed if resources are available. Conversely, avoid waking an application if it cannot proceed.\
-Agent uses binary semaphores and consists of three concurrent threads:\
Semaphore agentSem = 1\
	tobacco = 0\
	paper = 0\
	match = 0\
while (true) {\
	P(agentSem)\
	V(tobacco)\
	V(paper)\
}\
-*Make 3 agent threads sleep at max 200 milliseconds before beginning to wait on agentSem to make things more interesting.\
 -*Need 3 Boolean variables to indicate if an ingredient is on the table, 3 new semaphores to signal the smokers, and a semaphore for preserving mutual exclusion\
Boolean isTobacco = false, isPaper = false, isMatch = false;\
-Pusher code -- tobacco --> if finds paper, signal smoker with matches --> if finds matches, signal rolling papers --> if first pusher, set isTobacco:\
while (true) {\
	P(tobacco)\
	P(mutex)\
	if (isPaper) {\
		isPaper = false;\
		V(matchSem);\
	}\
	else if (isMatch) {\
		isMatch = false;\
		V(paperSem);\
	}\
	else isTobacco = true;\
	V(mutex);\
}\
-*Simulate the making and smoking of a cigarette by having the thread sleep for a short period of time (up to 50ms)\
while (true) {\
	P(tobaccoSem);\
	//Make a cigarette\
	V(agentSem);\
	//Smoke the cigarette\
}
# Summary
-3 agent threads\
-3 pushers\
-6 smokers: 2 tobacco, 2 papers, 2 matches\
	+Each smoker finishes 3 cigarettes before exiting\
	+Rather than loop forever, each agent loops 6 times, and each pusher 12 times.\
	+*Join all threads before the program terminates