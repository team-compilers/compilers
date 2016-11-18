class BinarySearch{
public static void main(String[] a) {
System.out.println(BS().Start(20));}
}
class BS {
int[]number;
intsize;
publicintStart(intsz) {
intaux01;
intaux02;
aux01 = this.Init(sz);aux02 = this.Print();if (this.Search(8)) System.out.println(1); else System.out.println(0);if (this.Search(19)) System.out.println(1); else System.out.println(0);if (this.Search(20)) System.out.println(1); else System.out.println(0);if (this.Search(21)) System.out.println(1); else System.out.println(0);if (this.Search(37)) System.out.println(1); else System.out.println(0);if (this.Search(38)) System.out.println(1); else System.out.println(0);if (this.Search(39)) System.out.println(1); else System.out.println(0);if (this.Search(50)) System.out.println(1); else System.out.println(0);return 999;
}publicbooleanSearch(intnum) {
booleanbs01;
intright;
intleft;
booleanvar_cont;
intmedium;
intaux01;
intnt;
aux01 = 0;bs01 = false;right = number.length;right = right - 1;left = 0;var_cont = true;while (var_cont) {
medium = left + right;medium = this.Div(medium);aux01 = number[medium];if (num < aux01) right = medium - 1; else left = medium + 1;if (this.Compare(aux01num)) var_cont = false; else var_cont = true;if (right < left) var_cont = false; else nt = 0;
}if (this.Compare(aux01num)) bs01 = true; else bs01 = false;return bs01;
}publicintDiv(intnum) {
intcount01;
intcount02;
intaux03;
count01 = 0;count02 = 0;aux03 = num - 1;while (count02 < aux03) {
count01 = count01 + 1;count02 = count02 + 2;
}return count01;
}publicbooleanCompare(intnum1intnum2) {
booleanretval;
intaux02;
retval = false;aux02 = num2 + 1;if (num1 < num2) retval = false; else if (!num1 < aux02) retval = false; else retval = true;return retval;
}publicintPrint() {
intj;
j = 1;while (j < size) {
System.out.println(number[j]);j = j + 1;
}System.out.println(99999);return 0;
}publicintInit(intsz) {
intj;
intk;
intaux02;
intaux01;
size = sz;number = new int[sz];j = 1;k = size + 1;while (j < size) {
aux01 = 2 * j;aux02 = k - 3;number[j] = aux01 + aux02;j = j + 1;k = k - 1;
}return 0;
}}
