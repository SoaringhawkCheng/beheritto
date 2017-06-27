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
**程序处理级别**||||
**DeclProgram**||||
=>|programParser()|FROM|FROM ID IMPORT ID **alias** **DeclFrom** **DeclProgram**|导入模版的类
=>|programParser()|FROM|FROM ID IMPORT ID **alias** **DeclFrom** **DeclProgram**|导入模版方法
=>|programParser()|IMPORT|IMPORT ID **DeclModule** **DeclProgram**|导入模版文件
=>|programParser()|CLASS|CLASS **DeclClass** **DeclProgram**|声明类
=>|programParser()|DEF|DEF ID **DeclMethod** **DeclProgram**|声明函数
=>|programParser()|IF|IF **DeclEntry** INDENT **DeclProgram**|声明入口函数
=>|programParser()|EOF|ε|
**DeclFrom**|
=>|fromParser()|ID|CLASS ID : **DelClass**|匹配为类
=>|fromParser()|ID|DEF ID **DeclMethod**|匹配为函数
**模版处理级别**||||
**DeclModule**||||
=>|moduleParser()|CLASS|CLASS **DeclClass** **DeclModule**|类声明
=>|moduleParser()|DEF|DEF **DeclMethod** **DeclModule**|函数声明
=>|moduleParser()|EOF|ε|
**类处理级别**||||
**DeclClass**|
=>|classParser()|-|ID : EOL INDENT **DeclClassP**|类定义
**DeclClassP**|
=>|classParser()|INIT|DEF INIT : **DeclField** DEDENT **DeclClassP**|类定义
=>|classParser()|ID|DEF ID **DeclMethod** **DeclClassP**|类定义
**DeclEntry**|
=>|entryParser()|-|DEF **StmtArgList** : **StmtBlock**|主函数
**函数处理级别**||||
**DeclConstructor**|
=>|MethodParser()|ID|DEF INIT **StmtParaList**: **StmtBlockC**|函数定义
**DeclMethod**|
=>|MethodParser()|ID|DEF ID **StmtParaList**: **StmtBlock**|函数定义
**块处理级别**|
**StmtBlock**|
=>|blockParser()||**Statment** **StmtBlock**|
=>|blockParser()|DEDENT|ε
**Statement**|
=>|statementParser()|IF|**StmtIf**
=>|statementParser()|WHILE|**StmtWhile**
=>|statementParser()|FOR|**StmtFor**
=>|statementParser()|RETURN|**StmtReturn**
=>|statementParser()|PRINT|**StmtPrint**
=>|statementParser()|ID|**StatementP**
**语句处理级别**|
**StmtIf**|
=>|ifParser()|-|IF **Expr** : **StatementBlock** **StatementP**|
**StatementP**|
=>|ifParser()|ELIF
**StmtElif**|
=>|elifParser()|-|ELIF **Expr** : **StatementBlock** |
=>|elifParser()|-|ε
**StmtElse**|
=>|elseParser()|-|ELSE : **StatementBlock**
=>|elseParser()|-|ε
**StmtWhile**|
=>|whileParser()|-|WHILE **ExprCondition** : **StatementBlock** **StatementElse**
**StmtFor**|
=>|forParser()|RANGE|FOR ID IN **RANGE** : INDENT **StatementBlock**
=>|forParser()|ID|FOR ID IN ID : INDENT **StatementBlock**
**StmtReturn**|
=>|returnParser()|-|RETURN **Expr**
**StmtPrint**|
=>|printParser()|(|PRINT ( **StmtExprList** )|
**StmtExprList**|
=>|exprlistParser()|isExpr|**Expr** , **StmtExprList**|
=>|exprlistParser()|)|ε

**运算处理级别**|
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