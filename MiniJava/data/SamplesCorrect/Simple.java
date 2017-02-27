class Main{
    public static void main(String[] a){
    System.out.println(10);
    }
}

class Fac {
    boolean num_aux;
    public int Sum(int num, int num2) {
        return num + num2;
    }

    public int Compute(int num){
    int num_aux ;

    num_aux = this.Sum( this.Compute(1), this.Compute(2) );
    if (num < 1)
        num_aux = 1 ;
    else 
        num_aux = num * num_aux ;
    return num_aux ;
    }

}
