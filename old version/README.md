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
=>|classParser()|.|ID : EOL INDENT **DeclClassP**|类定义
**DeclClassP**|
=>|classParser()|INIT|DEF INIT : **DeclField** DEDENT **DeclClassP**|类定义
=>|classParser()|ID|DEF ID **DeclMethod** **DeclClassP**|类定义
**DeclEntry**|
=>|entryParser()|.|DEF **StmtParaList** : **StmtBlock**|主函数
**函数处理级别**||||
**DeclConstructor**|
=>|MethodParser()|ID|DEF INIT **StmtParaList**: **StmtConstructBlock**|函数定义
**DeclMethod**|
=>|MethodParser()|ID|DEF ID **StmtParaList**: **StmtBlock**|函数定义
**块处理级别**|
**StmtBlock**|
=>|blockParser()|.|**Statment** **StmtBlock**|
=>|blockParser()|DEDENT|ε
**Statement**|
=>|statementParser()|IF|**StmtIf**|
=>|statementParser()|WHILE|**StmtWhile**|
=>|statementParser()|FOR|**StmtFor**|
=>|statementParser()|RETURN|**StmtReturn**|
=>|statementParser()|PRINT|**StmtPrint**|
=>|statementParser()|ID|**StatementP**|
**语句处理级别**|
**StmtIf**|
=>|ifParser()|.|IF **Expr** : INDENT **StatementBlock** **StmtElif** **StmtElse**|
**StmtElif**|
=>|elifParser()|.|ELIF **Expr** : INDENT **StatementBlock** |
=>|elifParser()|.|ε
**StmtElse**|
=>|elseParser()|.|ELSE : INDENT **StatementBlock**
=>|elseParser()|.|ε
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
**StatementP**|
=>|statementPParser()|=|= **Expr**|
=>|statementPParser()|[|**StmtArray** = **Expr**|
=>|statementPParser()|(|**StmtMethodCall**|
**运算处理级别**|
**Expr**|
=>|statementPParser()|'-'|**ExprOpposite**|
=>|statementPParser()|^-|= **Logical**|
=>|statementPParser()|=|= **Expr**|
=>|statementPParser()|=|= **Expr**|
=>|statementPParser()|=|= **Expr**|
=>|statementPParser()|=|= **Expr**|
=>|statementPParser()|=|= **Expr**|
=>|statementPParser()|=|= **Expr**|
=>|statementPParser()|=|= **Expr**|
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

## ベヘリット Beheritto 语法细节

类不支持继承，支持构造函数，类的的成员私有，类方法公有

变量只有赋值才会分配空间，禁止全局变量
