/*Semaphore agentSem = 1
	tobacco = 0
	paper = 0
	match = 0

while (true) {
	P(agentSem)
	V(tobacco)
	V(paper)
}

Boolean isTobacco = false, isPaper = false, isMatch = false;

while (true) {
	P(tobacco)
	P(mutex)
	if (isPaper) {
		isPaper = false;
		V(matchSem);
	}
	else if (isMatch) {
		isMatch = false;
		V(paperSem);
	}
	else isTobacco = true;
	V(mutex);
}

while (true) {
	P(tobaccoSem);
	//Make a cigarette
	V(agentSem);
	//Smoke the cigarette
}*/
