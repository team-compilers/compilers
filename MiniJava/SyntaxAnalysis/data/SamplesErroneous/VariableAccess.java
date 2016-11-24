class Main {
	public static void main(String[] args) {
	}
}

class Undefined {
	public int f() {
		return x;
	}
}

class Local {
	public int f() {
		int x;
		return x;
	}
}

class Argument {
	public int f(int x) {
		return x;
	}
}

class Field {
	int x;
	
	public int f() {
		return x;
	}
}