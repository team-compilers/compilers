class LinkedList{
public static void main(String[] a) {
System.out.println(LL().Start());}
}
class Element {
intAge;
intSalary;
booleanMarried;
publicbooleanInit(intv_Ageintv_Salarybooleanv_Married) {
Age = v_Age;Salary = v_Salary;Married = v_Married;return true;
}publicintGetAge() {
return Age;
}publicintGetSalary() {
return Salary;
}publicbooleanGetMarried() {
return Married;
}publicbooleanEqual(Elementother) {
booleanret_val;
intaux01;
intaux02;
intnt;
ret_val = true;aux01 = other.GetAge();if (!this.Compare(aux01Age)) ret_val = false; else {
aux02 = other.GetSalary();if (!this.Compare(aux02Salary)) ret_val = false; else if (Married) if (!other.GetMarried()) ret_val = false; else nt = 0; else if (other.GetMarried()) ret_val = false; else nt = 0;
}return ret_val;
}publicbooleanCompare(intnum1intnum2) {
booleanretval;
intaux02;
retval = false;aux02 = num2 + 1;if (num1 < num2) retval = false; else if (!num1 < aux02) retval = false; else retval = true;return retval;
}}
class List {
Elementelem;
Listnext;
booleanend;
publicbooleanInit() {
end = true;return true;
}publicbooleanInitNew(Elementv_elemListv_nextbooleanv_end) {
end = v_end;elem = v_elem;next = v_next;return true;
}publicListInsert(Elementnew_elem) {
booleanret_val;
Listaux03;
Listaux02;
aux03 = this;aux02 = List();ret_val = aux02.InitNew(new_elemaux03false);return aux02;
}publicbooleanSetNext(Listv_next) {
next = v_next;return true;
}publicListDelete(Elemente) {
Listmy_head;
booleanret_val;
booleanaux05;
Listaux01;
Listprev;
booleanvar_end;
Elementvar_elem;
intaux04;
intnt;
my_head = this;ret_val = false;aux04 = 0 - 1;aux01 = this;prev = this;var_end = end;var_elem = elem;while (!var_end && !ret_val) {
if (e.Equal(var_elem)) {
ret_val = true;if (aux04 < 0) {
my_head = aux01.GetNext();
} else {
System.out.println(0 - 555);aux05 = prev.SetNext(aux01.GetNext());System.out.println(0 - 555);
}
} else nt = 0;if (!ret_val) {
prev = aux01;aux01 = aux01.GetNext();var_end = aux01.GetEnd();var_elem = aux01.GetElem();aux04 = 1;
} else nt = 0;
}return my_head;
}publicintSearch(Elemente) {
intint_ret_val;
Listaux01;
Elementvar_elem;
booleanvar_end;
intnt;
int_ret_val = 0;aux01 = this;var_end = end;var_elem = elem;while (!var_end) {
if (e.Equal(var_elem)) {
int_ret_val = 1;
} else nt = 0;aux01 = aux01.GetNext();var_end = aux01.GetEnd();var_elem = aux01.GetElem();
}return int_ret_val;
}publicbooleanGetEnd() {
return end;
}publicElementGetElem() {
return elem;
}publicListGetNext() {
return next;
}publicbooleanPrint() {
Listaux01;
booleanvar_end;
Elementvar_elem;
aux01 = this;var_end = end;var_elem = elem;while (!var_end) {
System.out.println(var_elem.GetAge());aux01 = aux01.GetNext();var_end = aux01.GetEnd();var_elem = aux01.GetElem();
}return true;
}}
class LL {
publicintStart() {
Listhead;
Listlast_elem;
booleanaux01;
Elementel01;
Elementel02;
Elementel03;
last_elem = List();aux01 = last_elem.Init();head = last_elem;aux01 = head.Init();aux01 = head.Print();el01 = Element();aux01 = el01.Init(2537000false);head = head.Insert(el01);aux01 = head.Print();System.out.println(10000000);el01 = Element();aux01 = el01.Init(3942000true);el02 = el01;head = head.Insert(el01);aux01 = head.Print();System.out.println(10000000);el01 = Element();aux01 = el01.Init(2234000false);head = head.Insert(el01);aux01 = head.Print();el03 = Element();aux01 = el03.Init(2734000false);System.out.println(head.Search(el02));System.out.println(head.Search(el03));System.out.println(10000000);el01 = Element();aux01 = el01.Init(2835000false);head = head.Insert(el01);aux01 = head.Print();System.out.println(2220000);head = head.Delete(el02);aux01 = head.Print();System.out.println(33300000);head = head.Delete(el01);aux01 = head.Print();System.out.println(44440000);return 0;
}}
