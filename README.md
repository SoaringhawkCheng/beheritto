## ベヘリット Beheritto 状态转移图

状态|状态意义|' '|_|a-z|0-9|.|"|+|-|*|/|%|=|>|<|!|:|,|(|)|[|]|
---|---|---|---|---|---|---|---|---|---|---|
S0|新标记|S0|
S1|是ID|
S2|是数值|
S3||
S4||
S5|是字符|
S6||
S7||
S8||
S9||
S10||
S11||
S12||
S13||
S14||
S-1||

## ベヘリット Beheritto 语法生成式

起始节点|分析函数|产生规则|语法含义
------|------|------|------
**Procudure:**|
=>|treeParser()|**DefinitionClass** Procudure|类声明
=>|treeParser()|**DefinitionFunction** Procudure|函数声明
=>|treeParser()|**StatementAssignment** Procudure|全局变量
**DefinitionClass:**|
=>|classParser()|DEF ID **StatementArgs** : **StatementBlock** END|类定义
**DefinitionFunc:**|
=>|functionParser()|DEF MAIN **StatementArgs** : **StatementBlock** END|主函数
=>|functionParser()|DEF VAR StatementArgs : **StatementBlock** END|函数定义
**StatementBlock:**|
=>|blockParser()|**StatementAssignment** **StatementBlock**|
=>|blockParser()|**StatementMethodCall** **StatementBlock**|
=>|blockParser()|**StatementIf** **StatementBlock**|
=>|blockParser()|**StatementWhile** **StatementBlock**|
=>|blockParser()|**StatementFor** **StatementBlock**|
=>|blockParser()|**StatementReturn** **StatementBlock**|
=>|blockParser()|**StatementInput** **StatementBlock**|
=>|blockParser()|**StatementPrint** **StatementBlock**|
=>|blockParser()|ε
**StatementAssignment:**|
=>|statementParser()|**LValue** = **RValue**
**StatementMethodCall:**|
=>|statementParser()|VAR ( **StatementArgList** )
=>|statementParser()|VAR ( **StatementInput** )
=>|statementParser()|VAR ( **StatementPrint** )
**StatementIf:**|
=>|statementParser()|if **Expr** : **elif** **StatementPrint**
=>|statementParser()|**StatementPrint**
=>|statementParser()|**StatementPrint**
=>|statementParser()|**StatementPrint**
=>|statementParser()|**StatementPrint**
=>|statementParser()|**StatementPrint**
=>|statementParser()|**StatementPrint**
=>|statementParser()|print ( **Expr** )
=>|statementParser()|if **Expr** (elif **Expr** : **StatementBlock** )* else : **StatementBlock**
=>|statementParser()|while **Expr** : **StatementBlock**
=>|statementParser()|for VAR in range ( ==Expr== )
=>|statementParser()|return ==Expr==
=>|statementParser()|break
StatementArgs:|
=>|argsParser()|( Statement )
ExprSlice:|
=>|sliceParser()|[ lice ]
ExprInitializer:|
=>|Parser()|{ Initializer }
Initializer:|
=>|argsParser()|Term , Initializer
=>|argsParser()|ε
=>||
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