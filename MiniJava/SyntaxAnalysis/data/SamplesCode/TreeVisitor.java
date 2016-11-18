class TreeVisitor{
public static void main(String[] a) {
System.out.println(TV().Start());}
}
class TV {
publicintStart() {
Treeroot;
booleanntb;
intnti;
MyVisitorv;
root = Tree();ntb = root.Init(16);ntb = root.Print();System.out.println(100000000);ntb = root.Insert(8);ntb = root.Insert(24);ntb = root.Insert(4);ntb = root.Insert(12);ntb = root.Insert(20);ntb = root.Insert(28);ntb = root.Insert(14);ntb = root.Print();System.out.println(100000000);v = MyVisitor();System.out.println(50000000);nti = root.accept(v);System.out.println(100000000);System.out.println(root.Search(24));System.out.println(root.Search(12));System.out.println(root.Search(16));System.out.println(root.Search(50));System.out.println(root.Search(12));ntb = root.Delete(12);ntb = root.Print();System.out.println(root.Search(12));return 0;
}}
class Tree {
Treeleft;
Treeright;
intkey;
booleanhas_left;
booleanhas_right;
Treemy_null;
publicbooleanInit(intv_key) {
key = v_key;has_left = false;has_right = false;return true;
}publicbooleanSetRight(Treern) {
right = rn;return true;
}publicbooleanSetLeft(Treeln) {
left = ln;return true;
}publicTreeGetRight() {
return right;
}publicTreeGetLeft() {
return left;
}publicintGetKey() {
return key;
}publicbooleanSetKey(intv_key) {
key = v_key;return true;
}publicbooleanGetHas_Right() {
return has_right;
}publicbooleanGetHas_Left() {
return has_left;
}publicbooleanSetHas_Left(booleanval) {
has_left = val;return true;
}publicbooleanSetHas_Right(booleanval) {
has_right = val;return true;
}publicbooleanCompare(intnum1intnum2) {
booleanntb;
intnti;
ntb = false;nti = num2 + 1;if (num1 < num2) ntb = false; else if (!num1 < nti) ntb = false; else ntb = true;return ntb;
}publicbooleanInsert(intv_key) {
Treenew_node;
booleanntb;
Treecurrent_node;
booleancont;
intkey_aux;
new_node = Tree();ntb = new_node.Init(v_key);current_node = this;cont = true;while (cont) {
key_aux = current_node.GetKey();if (v_key < key_aux) {
if (current_node.GetHas_Left()) current_node = current_node.GetLeft(); else {
cont = false;ntb = current_node.SetHas_Left(true);ntb = current_node.SetLeft(new_node);
}
} else {
if (current_node.GetHas_Right()) current_node = current_node.GetRight(); else {
cont = false;ntb = current_node.SetHas_Right(true);ntb = current_node.SetRight(new_node);
}
}
}return true;
}publicbooleanDelete(intv_key) {
Treecurrent_node;
Treeparent_node;
booleancont;
booleanfound;
booleanntb;
booleanis_root;
intkey_aux;
current_node = this;parent_node = this;cont = true;found = false;is_root = true;while (cont) {
key_aux = current_node.GetKey();if (v_key < key_aux) if (current_node.GetHas_Left()) {
parent_node = current_node;current_node = current_node.GetLeft();
} else cont = false; else if (key_aux < v_key) if (current_node.GetHas_Right()) {
parent_node = current_node;current_node = current_node.GetRight();
} else cont = false; else {
if (is_root) if (!current_node.GetHas_Right() && !current_node.GetHas_Left()) ntb = true; else ntb = this.Remove(parent_nodecurrent_node); else ntb = this.Remove(parent_nodecurrent_node);found = true;cont = false;
}is_root = false;
}return found;
}publicbooleanRemove(Treep_nodeTreec_node) {
booleanntb;
intauxkey1;
intauxkey2;
if (c_node.GetHas_Left()) ntb = this.RemoveLeft(p_nodec_node); else if (c_node.GetHas_Right()) ntb = this.RemoveRight(p_nodec_node); else {
auxkey1 = c_node.GetKey();auxkey2 = p_node.GetLeft().GetKey();if (this.Compare(auxkey1auxkey2)) {
ntb = p_node.SetLeft(my_null);ntb = p_node.SetHas_Left(false);
} else {
ntb = p_node.SetRight(my_null);ntb = p_node.SetHas_Right(false);
}
}return true;
}publicbooleanRemoveRight(Treep_nodeTreec_node) {
booleanntb;
while (c_node.GetHas_Right()) {
ntb = c_node.SetKey(c_node.GetRight().GetKey());p_node = c_node;c_node = c_node.GetRight();
}ntb = p_node.SetRight(my_null);ntb = p_node.SetHas_Right(false);return true;
}publicbooleanRemoveLeft(Treep_nodeTreec_node) {
booleanntb;
while (c_node.GetHas_Left()) {
ntb = c_node.SetKey(c_node.GetLeft().GetKey());p_node = c_node;c_node = c_node.GetLeft();
}ntb = p_node.SetLeft(my_null);ntb = p_node.SetHas_Left(false);return true;
}publicintSearch(intv_key) {
Treecurrent_node;
intifound;
booleancont;
intkey_aux;
current_node = this;cont = true;ifound = 0;while (cont) {
key_aux = current_node.GetKey();if (v_key < key_aux) if (current_node.GetHas_Left()) current_node = current_node.GetLeft(); else cont = false; else if (key_aux < v_key) if (current_node.GetHas_Right()) current_node = current_node.GetRight(); else cont = false; else {
ifound = 1;cont = false;
}
}return ifound;
}publicbooleanPrint() {
booleanntb;
Treecurrent_node;
current_node = this;ntb = this.RecPrint(current_node);return true;
}publicbooleanRecPrint(Treenode) {
booleanntb;
if (node.GetHas_Left()) {
ntb = this.RecPrint(node.GetLeft());
} else ntb = true;System.out.println(node.GetKey());if (node.GetHas_Right()) {
ntb = this.RecPrint(node.GetRight());
} else ntb = true;return true;
}publicintaccept(Visitorv) {
intnti;
System.out.println(333);nti = v.visit(this);return 0;
}}
class Visitor {
Treel;
Treer;
publicintvisit(Treen) {
intnti;
if (n.GetHas_Right()) {
r = n.GetRight();nti = r.accept(this);
} else nti = 0;if (n.GetHas_Left()) {
l = n.GetLeft();nti = l.accept(this);
} else nti = 0;return 0;
}}
class MyVisitor extends Visitor {
publicintvisit(Treen) {
intnti;
if (n.GetHas_Right()) {
r = n.GetRight();nti = r.accept(this);
} else nti = 0;System.out.println(n.GetKey());if (n.GetHas_Left()) {
l = n.GetLeft();nti = l.accept(this);
} else nti = 0;return 0;
}}
