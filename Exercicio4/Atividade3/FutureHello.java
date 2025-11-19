package Entities;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class FutureHello {
	private static final int NTHREADS = 10;
	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		System.out.print("Digite o valor de N: ");
		long N = in.nextLong();
		if (N < 1) {
			System.out.println("N deve ser um número inteiro positivo");
			in.close();
			return;
		}
		in.close();
		ExecutorService executor = Executors.newFixedThreadPool(NTHREADS);
		List<Future<Integer>> lista = new ArrayList<>();
		for (long i = 1; i <= N; i++) {
			Future<Integer> futuro = executor.submit(new ChecarPrimo(i));
			lista.add(futuro);
		}
		long totalPrimos = 0;
		for (Future<Integer> f : lista) {
			try {
				totalPrimos += f.get();
			}
			catch (ExecutionException | InterruptedException e) {
				e.printStackTrace();
			}
		}
		executor.shutdown();
		System.out.println("Quantidade de números primos entre 1 e " + N + ": " + totalPrimos);
		
	}

}
