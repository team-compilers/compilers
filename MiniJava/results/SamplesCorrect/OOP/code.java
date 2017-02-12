class Main {
    public static void main(String[] a) {
        System.out.println(new GrandChild().Func(2));
    }
}

class Base {
    int size;
    public int Func(int num) {
        return 1;
    }

}
class Child extends Base{
}
class GrandChild extends Child{
    private int Add() {
        return size + 1;
    }

}

