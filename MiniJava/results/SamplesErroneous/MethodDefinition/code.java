class Main {
    public static void main(String[] args) {
    }
}

class Base {
    public Random f() {
        return 0;
    }

    public int f() {
        return 0;
    }

}
class Middle extends Base{
}
class Child extends Middle{
    public int f() {
        return 0;
    }

}

