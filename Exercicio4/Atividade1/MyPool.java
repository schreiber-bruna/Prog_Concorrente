package Entities;

public class MyPool {
	private static final int NTHREADS = 10;
	public static void main (String[] args) {
		FilaTarefas pool = new FilaTarefas(NTHREADS);
		for (int i = 0; i < 25; i++) {
			Runnable primo = new Primo(i);
			pool.execute(primo);
		}
		pool.shutdown();
		System.out.println("Terminou");
	}

}
