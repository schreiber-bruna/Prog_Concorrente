package Entities;

public class Hello implements Runnable{
	String msg;
	public Hello(String m) { msg = m; }
	public void run() {
		System.out.println(msg);
	}

}
