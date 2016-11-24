class Main {
	public static void main(String[] args) {
	}
}

class A {
	private int f() {
		return 0;
	}

	public int g() {
		return 0;
	}

	public int h(int x) {
		return 0;
	}
}

class B {
	private int g() {
		return 0;
	}

	public int f() {
		int x;
		A a;
		B b;

		a = new A();
		b = new B();

		x = a.f();
		
		x = a.g();

		x = a.h();
		x = a.h(2, 5);
		x = a.h(true);
		
		x = b.g();
		return 0;
	}
}