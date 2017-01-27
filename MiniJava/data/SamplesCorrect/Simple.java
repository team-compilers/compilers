class Main{
    public static void main(String[] a){
    System.out.println(10);
    }
}

class Fac {

    public int Compute(int num){
    int num_aux ;
    if (num < 1)
        num_aux = 1 ;
    else 
        num_aux = num * num_aux ;
    return num_aux ;
    }

}
