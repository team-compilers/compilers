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
class Visitor {
    public int visit(Tree n) {
        return 0;
    }

    public int visit(TV v) {
        return 0;
    }

}

