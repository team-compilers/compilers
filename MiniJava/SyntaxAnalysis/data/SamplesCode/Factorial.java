class Factorial{
public static void main(String[] a) {
System.out.println(Fac().ComputeFac(10));}
}
class Fac {
publicintComputeFac(intnum) {
intnum_aux;
if (num < 1) num_aux = 1; else num_aux = num * this.ComputeFac(num - 1);return num_aux;
}}
