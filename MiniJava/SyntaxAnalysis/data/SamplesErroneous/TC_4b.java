// The classes are basically the same as the BinaryTree 
// file except the visitor classes and the accept method
// in the Tree class

class TreeVisitor{
    public static void main(String[] a){
	System.out.println(new TV().Start());
    }
}

class TV {

    public int Start(){

	return 0 ;
    }

}



class Visitor {

    public int visit(Tree n, Tree2 n){       // HERE
		return 0;
    }

}
