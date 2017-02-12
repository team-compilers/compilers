class TreeVisitor {
    public static void main(String[] a) {
        System.out.println(new TV().Start());
    }
}

class TV {
    public int Start() {
        return 0;
    }

}
class Visitor extends MyVisitor{
}
class MyVisitor extends Visitor{
}

