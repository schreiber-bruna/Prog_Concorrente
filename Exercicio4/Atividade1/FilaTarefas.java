package Entities;

import java.util.LinkedList;

public class FilaTarefas {
	
	private final int nThreads;
    private final MyPoolThreads[] threads;
    private final LinkedList<Runnable> queue;
    private boolean shutdown;
    
    public FilaTarefas(int nThreads) {
    	this.shutdown = false;
        this.nThreads = nThreads;
        queue = new LinkedList<Runnable>();
        threads = new MyPoolThreads[nThreads];
        for (int i=0; i<nThreads; i++) {
            threads[i] = new MyPoolThreads();
            threads[i].start();
        } 
    }
    
    public void execute(Runnable r) {
    	synchronized (queue) {
    		if (this.shutdown) {
    			return;
    		}
    		queue.addLast(r);
    		queue.notify();
    	}
    }
    
    public void shutdown() {
    	synchronized(queue) {
            this.shutdown=true;
            queue.notifyAll();
        }
        for (int i=0; i<nThreads; i++) {
          try { threads[i].join(); } catch (InterruptedException e) { return; }
        }
    }
    
    private class MyPoolThreads extends Thread {
    	public void run() {
    		Runnable r;
    		while (true) {
    			synchronized (queue) {
    				while (queue.isEmpty() && (!shutdown)) {
    					try { queue.wait(); }
    					catch (InterruptedException ignored) {}
    				}
    				if (queue.isEmpty()) return;
    				r = queue.removeFirst();
    				
    			}
    			try { r.run(); }
    			catch (RuntimeException e) {}
    		}
    	}
    }
}
