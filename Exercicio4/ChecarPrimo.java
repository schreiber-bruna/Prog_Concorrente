package Entities;

import java.util.concurrent.Callable;

public class ChecarPrimo implements Callable<Integer>{
	private final long n;

    public ChecarPrimo(long n) {
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
    public Integer call() {
        return ehPrimo(n) ? 1 : 0;
    }
}
