#include "collector.h"

GarbageCollector *GarbageCollector::gc=NULL;

GarbageCollector * GarbageCollector::getInstance(){
    if(gc==NULL)
        gc=(GarbageCollector *)malloc(sizeof(GarbageCollector));
    return gc;
}

void GarbageCollector::process(){
    cout<<"Garbage collector is running!"<<endl;
    gc->clear();
    gc->mark();
    gc->sweep();
    cout<<"Garbage collector stops!"<<endl;
}

void GarbageCollector::mark(){
    if(state==StateType::STATEANALYZE) markAnalyze(program);
    if(state==StateType::STATERUNTIME) markRuntime(runtimestack);
}

void GarbageCollector::markAnalyze(DeclModule *program){
    for(auto iter=program->modulelist.begin();iter!=program->modulelist.end();++iter)
        mark((*iter).second);
    for(auto iter=program->classlist.begin();iter!=program->classlist.end();++iter)
        mark((*iter).second);
    for(auto iter=program->methodlist.begin();iter!=program->methodlist.end();++iter)
        mark((*iter).second);
    if(program->entry) mark(program->entry);
    
}

void GarbageCollector::markRuntime(RuntimeStack runtimestack){
    for(auto stack_iter=runtimestack.stackframelist.begin();stack_iter!=runtimestack.stackframelist.end();++stack_iter){
        for(auto vartable_iter=(*stack_iter)->variabletable.begin();vartable_iter!=(*stack_iter)->variabletable.end();++vartable_iter){
            mark((*vartable_iter).second->object);
        }
    }
}

void GarbageCollector::mark(ASTree *node){
    if(node){
        int nodetype=node->getNodeType();
        if(nodetype==NodeType::NODEEXPR){
            Expression *expr=dynamic_cast<Expression *>(node);
            int exprtype=expr->getExprType();
            if(exprtype==ExprType::EXPROPUN){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark){
                    meta->mark=true;
                    ExprOpUnary *expropun=(ExprOpUnary *)node;
                    mark(expropun->expr);
                }
                return;
            }
            if(exprtype==ExprType::EXROPBIN){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark){
                    meta->mark=true;
                    ExprOpBinary *expropbin=(ExprOpBinary *)node;
                    mark(expropbin->lexpr);
                    mark(expropbin->rexpr);
                }
                return;
            }
            if(exprtype==ExprType::EXPRLVALUE){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark)
                    meta->mark=true;
                return;
            }
            if(exprtype==ExprType::EXPRCONSTANT){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark)
                    meta->mark=true;
                return;
            }
            if(exprtype==ExprType::EXPRARRAYINIT){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark){
                    meta->mark=true;
                    ExprArrayInit *exprarrayinit=(ExprArrayInit *)node;
                    for(auto iter=exprarrayinit->initlist.begin();iter!=exprarrayinit->initlist.end();++iter)
                        mark((*iter));
                }
                return;
            }
            if(exprtype==ExprType::EXPRRANGE){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark){
                    meta->mark=true;
                    ExprRange *exprrange=(ExprRange *)node;
                    mark(exprrange->begin);
                    mark(exprrange->end);
                    mark(exprrange->step);
                }
                return;
            }
            if(exprtype==ExprType::EXPRMETHODCALL){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark){
                    meta->mark=true;
                    ExprMethodCall *exprmethodcall=(ExprMethodCall *)node;
                    for(auto iter=exprmethodcall->arglist.begin();iter!=exprmethodcall->arglist.end();++iter)
                        mark(*iter);
                }
                return;
            }
            if(exprtype==ExprType::EXPRINPUT){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark)
                    meta->mark=true;
                return;
            }
        }
        if(nodetype==NodeType::NODESTMT){
            Statement *statement=dynamic_cast<Statement *>(node);
            int stmttype=statement->getStmtType();
            if(stmttype==StmtType::STMTBLOCK){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark){
                    meta->mark=true;
                    StmtBlock *stmtblock=dynamic_cast<StmtBlock *>(statement);
                    for(auto iter=stmtblock->statements.begin();iter!=stmtblock->statements.end();++iter)
                        mark(*iter);
                }
                return;
            }
            if(stmttype==StmtType::STMTASSIGN){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark){
                    meta->mark=true;
                    StmtAssign *stmtassign=(StmtAssign *)node;
                    mark(stmtassign->lexpr);
                    mark(stmtassign->rexpr);
                }
                return;
            }
            if(stmttype==StmtType::STMTMETHODCALL){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark){
                    meta->mark=true;
                    StmtMethodCall *stmtmethodcall=(StmtMethodCall *)node;
                    mark(stmtmethodcall->methodcall);
                }
                return;
            }
            if(stmttype==StmtType::STMTIF){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark){
                    meta->mark=true;
                    StmtIf *stmtif=(StmtIf *)node;
                    mark(stmtif->condition);
                    mark(stmtif->ifblock);
                    mark(stmtif->elseblock);
                    for(auto iter=stmtif->eliflist.begin();iter!=stmtif->eliflist.end();++iter)
                        mark(*iter);
                }
                return;
            }
            if(stmttype==StmtType::STMTELIF){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark){
                    meta->mark=true;
                    StmtElif *stmtelif=(StmtElif *)node;
                    mark(stmtelif->condition);
                    mark(stmtelif->elifblock);
                }
                return;
            }
            if(stmttype==StmtType::STMTWHILE){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark){
                    meta->mark=true;
                    StmtWhile *stmtwhile=(StmtWhile *)node;
                    mark(stmtwhile->condition);
                    mark(stmtwhile->whileblock);
                }
                return;
            }
            if(stmttype==StmtType::STMTFOR){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark){
                    meta->mark=true;
                    StmtFor *stmtfor=(StmtFor *)node;
                    mark(stmtfor->forblock);
                    mark(stmtfor->range);
                }
                return;
            }
            if(stmttype==StmtType::STMTRETURN){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark){
                    meta->mark=true;
                    StmtReturn *stmtreturn=(StmtReturn *)node;
                    mark(stmtreturn->ret);
                }
                return;
            }
            if(stmttype==StmtType::STMTBREAK){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark)
                    meta->mark=true;
                return;
            }
            if(stmttype==StmtType::STMTCONTINUE){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark)
                    meta->mark=true;
                return;
            }
        }
        if(nodetype==NodeType::NODEDECL){
            Declaration *declaration=dynamic_cast<Declaration *>(node);
            int decl_type=declaration->getDeclType();
            if(decl_type==DeclType::DECLMODULE){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark){
                    meta->mark=true;
                    DeclModule *declmodule=(DeclModule *)node;
                    for(auto iter=declmodule->modulelist.begin();iter!=declmodule->modulelist.end();++iter)
                        mark((*iter).second);
                    for(auto iter=declmodule->classlist.begin();iter!=declmodule->classlist.end();++iter)
                        mark((*iter).second);
                    for(auto iter=declmodule->methodlist.begin();iter!=declmodule->methodlist.end();++iter)
                        mark((*iter).second);
                    if(declmodule->entry) mark(declmodule->entry);
                }
                return;
            }
            if(decl_type==DeclType::DECLCLASS){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark){
                    meta->mark=true;
                    DeclClass *declclass=(DeclClass *)node;
                    for(auto iter=declclass->fieldlist.begin();iter!=declclass->fieldlist.end();++iter)
                        mark(*iter);
                    for(auto iter=declclass->methodlist.begin();iter!=declclass->methodlist.end();++iter)
                        mark((*iter).second);
                }
                return;
            }
            if(decl_type==DeclType::DECLMETHOD){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark){
                    meta->mark=true;
                    DeclMethod *declmethod=(DeclMethod *)node;
                    mark(declmethod->methodblock);
                    mark(declmethod->returnobj);
                }
                return;
            }
            if(decl_type==DeclType::DECLFIELD){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark){
                    meta->mark=true;
                    DeclField *declfield=(DeclField *)node;
                    mark(declfield->assign);
                }
                return;
            }
            if(decl_type==DeclType::DECLENTRY){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark){
                    meta->mark=true;
                    DeclEntry *declentry=(DeclEntry *)node;
                    for(auto iter=declentry->statements.begin();iter!=declentry->statements.end();++iter)
                        mark(*iter);
                }
                return;
            }
        }
        if(nodetype==NodeType::NODEOBJ){
            Object *object=dynamic_cast<Object *>(node);
            int objtype=object->getObjType();
            if(objtype==ObjType::OBJINTEGER){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark)
                    meta->mark=true;
                return;
            }
            if(objtype==ObjType::OBJFLOAT){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark)
                    meta->mark=true;
                return;
            }
            if(objtype==ObjType::OBJBOOLEAN){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark)
                    meta->mark=true;
                return;
            }
            if(objtype==ObjType::OBJSTRING){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark)
                    meta->mark=true;
                return;
            }
            if(objtype==ObjType::OBJARRAY){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark){
                    meta->mark=true;
                    ObjArray *objarray=(ObjArray *)node;
                    for(auto iter=objarray->value.begin();iter!=objarray->value.end();++iter)
                        mark(*iter);
                }
                return;
            }
            if(objtype==ObjType::OBJCLASS){
                void *buff=(void *)node;
                mempool->getBlockMeta(buff);
                MemBlockMeta *meta=(MemBlockMeta *)buff;
                if(!meta->mark){
                    meta->mark=true;
                    ObjClass *objclass=(ObjClass *)node;
                    for(auto iter=objclass->fieldlist.begin();iter!=objclass->fieldlist.end();++iter)
                        mark((*iter).second);
                    for(auto iter=objclass->methodlist.begin();iter!=objclass->methodlist.end();++iter)
                        mark((*iter).second);
                }
                return;
            }
        }
    }
}

void GarbageCollector::sweep(){
    void *buff=NULL;
    for(auto list_iter=mempool->lists.begin();list_iter!=mempool->lists.end();++list_iter){
        for(auto block_iter=(*list_iter)->used.begin();block_iter!=(*list_iter)->used.end();++block_iter){
            buff=(*block_iter)->block;
            mempool->getBlockMeta(buff);
            MemBlockMeta *meta=(MemBlockMeta *)buff;
            if(!meta->mark){
                MemBlock *memblock=*block_iter;
                (*list_iter)->used.erase(block_iter);
                (*list_iter)->unused.push_back(memblock);
                --block_iter;
                if((*list_iter)->used.empty()){
                    MemList *memlist=*list_iter;
                    while(!memlist->unused.empty()){
                        MemBlock *unusedmemblock=memlist->unused.back();
                        memlist->unused.pop_back();
                        free(unusedmemblock);
                    }
                    mempool->lists.erase(list_iter);
                    --list_iter;
                }
            }
        }
    }
}

void GarbageCollector::clear(){
    void *buff=NULL;
    for(auto list_iter=mempool->lists.begin();list_iter!=mempool->lists.end();++list_iter){
        for(auto block_iter=(*list_iter)->used.begin();block_iter!=(*list_iter)->used.end();++block_iter){
            buff=(*block_iter)->block;
            mempool->getBlockMeta(buff);
            MemBlockMeta *meta=(MemBlockMeta *)buff;
            meta->mark=false;
        }
    }
}
