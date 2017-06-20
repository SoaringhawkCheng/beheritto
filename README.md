## ベヘリット Beheritto 状态转移图

状态|状态意义|||||||||||
--|--|--|--|--|--|--|--|-
S0  | Content Cell
S1  | Content Cell

## ベヘリット Beheritto 语法生成式

起始节点|接口函数|产生规则|语法含义
------|------|------|------
Procudure|treeParser()|==DefineClass== Procudure|类声明
 |treeParser()|==DefineFunction== Procudure|函数声明
 |treeParser()|==StatementAssign== Procudure|全局变量
 |||
DefineClass|classParser()|def ==ID== ==StatementArgs== : ==StatementBlock== end|类定义
DefineFunction|functionParser()|def ==MAIN== ==StatementArgs== : ==StatementBlock== end|主函数定义
 |functionParser()|def ==ID== ==StatementArgs== : ==StatementBlock== end|函数定义
StatementBlock|blockParser()|==Statement==
 ||
 ||
 ||
 ||
 ||
 ||
 ||
Statement|statementParser()|==Assign==
 |statementParser()|print ( ==Expr== )
 |statementParser()|if ==Expr== (elif ==Expr== : ==Block== )* else : ==Block==
 |statementParser()|while ==Expr== : ==Block==
 |statementParser()|for ==Var== in range ( ==Expr== )
 |statementParser()|return ==Expr==
 |statementParser()|break
 ||
Assignment|assignParser()|Var ( [ Expr ( , ==Expr== ) ] )* = 
 |
 |
 |
 |
 |
 |
 |
 |
 |
 |
 |
 |
 |
 |
 |
 |
 |
 |
 |
 |
 |
 |
 |
 |
 |
 |
 |
 |

## ベヘリット Beheritto 编程范式

### Bubble Sort

```
def bubbleSort(arg):
	for i in 0 to arg.size-2:
		for j in 1 to arg.size-1:
			if arg[j-1] > arg[j]:
				tmp=arg[j-1];
				arg[j-1] = arg[j];
				arg[j] = tmp;

def main(arg):
	arr1=[1,4,2,8,5,7,9];
	front=arr[0];
	arr1.delete(0);
	end=arr1.pop();
	arr1.insert(0,front);
	arr2=[0,3,6,9];
	for num in arr2:
		arr1.app
	bubble Sort(arr);
	print(arr);
```
### tuple
```
def main(arg):
	arr=(1,2,3);
	print(arr[0]);
	print(arr[-1]);
	print()
```	

### list
```
def main(arg):
	arr=[1,2,3];
	print(arr[0]);
	print[arr[-1]];
	arr.remove()
	arr.append(5);
	arr.insert()
```	
### string
```
def main(arg):
	str1="Beheritto"+" ";
	str2="Go "*3;
	print(str1,str2);
```


### dict