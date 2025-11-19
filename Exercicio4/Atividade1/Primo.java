package Entities;

public class Primo implements Runnable{
	private final long n;
	public Primo(long n) {
		this.n = n;
	}
	private boolean ehPrimo(long x) {
		if (x <= 1) {
			return false;
		}
		if (x == 2) {
			return true;
		}
		if (x % 2 == 0) {
			return false;
		}
		for (long i = 3; i * i <= x; i += 2) {
			if (x % i == 0) {
				return false;
			}
		}
		return true;
	}
	@Override
	public void run() {
		if (ehPrimo(n))
			System.out.println(n + " é primo");
		else
			System.out.println(n + " NÃO é primo");
		
	}

}
