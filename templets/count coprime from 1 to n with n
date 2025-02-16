void precompute() {
	for (int i = 1; i < MAX_N; i++) { phi[i] = i; }
	for (int i = 2; i < MAX_N; i++) {
		// If i is prime
		if (phi[i] == i) {
			for (int j = i; j < MAX_N; j += i) { phi[j] -= phi[j] / i; }
		}
	}
}
phi[i] is number of numbers from 1 to n which is coprime to n mean gcd(a, n) = 1;

which called Euler's totient function 
