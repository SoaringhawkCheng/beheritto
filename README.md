## ベヘリット Beheritto 编程范式

```
class MyClass:
	def __init__(self,arg1,arg2,...):
		self.item=arg1;
		self.__item=arg2;
	def method(self,arg1,..):
		...
	def __method(self,arg1,...):
		...

def function(arg1,...):
	...

def main(arg1,...):
	object=Myclass(arg1,arg2,...)  
	object.method(arg1)		√
	object.__method(arg2)	X
	object.itemp=1
	a=[]
	b
	e=c+d
	a[-1]=f;
	a={1,2,3,4}
	a.append()
	a.sort()
	a.reverse()
	a.insert()
	a.pop()
	a.remove()
	g=a[1:3]
	f=function(arg1,...) 
	for i in range(::):
		...
	while i>=0:
		...
	if i in ...:
		...
	elif ...:
		...
	else
		...
	...
```

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
S14||vim
S-1||

## ベヘリット Beheritto 语法生成式

起始节点|分析函数|前看字符|产生规则|语法含义
------|------|------|------|-----|
**DeclProgram**||||主文件
=>|programParser()|FROM|FROM ID IMPORT ID **alias** **DeclClass** **DeclProgram**|导入模版的类
=>|programParser()|IMPORT|IMPORT ID **DeclModule** **DeclProgram**|导入模版文件
=>|programParser()|CLASS|CLASS ID : **DeclClass** **DeclProgram**|声明类
=>|programParser()|DEF|DEF **parameter** : **DeclMethod**|声明函数
=>|programParser()|IF|IF "__name__=="main":" **DeclMain**|声明入口函数
=>|programParser()|EOF|ε|
**alis**||||别名
=>|-|AS|AS ID|
=>|-|ε|ε|
=>|**DeclModule**|
=>|moduleParser()|CLASS|**DeclClass** **DeclModule**|类声明
=>|moduleParser()|DEF|**DeclMethod** **DeclModule**|函数声明
**DeclClass**|
=>|classParser()|DEF|CLASS ID : **StmtBlock**|类定义
**DeclFunction**|
=>|funcParser()|MAIN|DEF MAIN **StmtArgList** : **StmtBlock**|主函数
=>|funcParser()|ID|DEF ID **StmtArgList**: **StmtBlock**|函数定义
**StmtBlock**|
=>|-|blockParser()|**Statment** **StmtBlock**|
=>|-|-|ε
**Statement**|
=>|statementParser()|IF|**StmtIf**
=>|statementParser()|WHILE|**StmtWhile**
=>|statementParser()|FOR|**StmtFor**
=>|statementParser()|RETURN|**StmtReturn**
=>|statementParser()|INPUT|**StmtInput**
=>|statementParser()|PRINT|**StmtPrint**
=>|statementParser()|ID|**Statement**
=>|statementParser()|~|~ **Expr**
=>|statementParser()|-|- **Expr**
=>|statementParser()|ε
**StmtIf**|
=>|ifParser()|-|IF **Expr** : **StatementBlock** **StatementElif** **StmtElse** END
**StmtElif**|
=>|elifParser()|-|ELIF **Expr** : **StatementBlock** END
=>|elifParser()|-|ε
**StmtElse**|
=>|elseParser()|-|ELSE : **StatementBlock** END
=>|elseParser()|-|ε
**StmtWhile**|
=>|whileParser()|-|WHILE **Expr** : **StatementBlock** **StatementElse** END
**StmtFor**|
=>|whileParser()|-|FOR **ExprIDiable** IN **RANGE** : **StatementBlock** **StmtElse** END
**StmtReturn**|
=>|returnParser()|-|RETURN **Expr**
**StmtBreak**|
=>|continueParser()|-|BREAK
**StmtContinue**|
=>|printParser()|-|CONITINUE
**StmtPrint**|
=>|printParser()|-|IF **Expr** : **StmtBlock** END|
**Statement**|
=>|statPParser()|.|**ExprMethodCall**|
=>|statementPParser()|^.|**StatementP**|
**StatementP**|
=>|statementPParser()|=|= **Expr**|
=>|statementPParser()|(|**ExprFunCall**|
=>|statementPParser()|ε
**ExprFunCall**|
=>|funCallParser()||( **ArgList** )
**ArgList**|
=>|funCallParser|-|**Expr** , ArgList |
=>|-|-|ε
**StmtAssignment**||
=>|statementParser()|-|**ExprIDiable** **StatementAssignmentP**|
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
 |




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