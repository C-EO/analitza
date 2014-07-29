/*************************************************************************************
 *  Copyright (C) 2014 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com>      *
 *                                                                                   *
 *  This program is free software; you can redistribute it and/or                    *
 *  modify it under the terms of the GNU General Public License                      *
 *  as published by the Free Software Foundation; either version 2                   *
 *  of the License, or (at your option) any later version.                           *
 *                                                                                   *
 *  This program is distributed in the hope that it will be useful,                  *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    *
 *  GNU General Public License for more details.                                     *
 *                                                                                   *
 *  You should have received a copy of the GNU General Public License                *
 *  along with this program; if not, write to the Free Software                      *
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA   *
 *************************************************************************************/

#include "expressioncompiler.h"

#include <llvm/Analysis/Verifier.h>
#include <llvm/IR/IntrinsicInst.h>
#include <llvm/IR/IRBuilder.h>

#include "analitza/value.h"
#include "analitza/matrix.h"
#include "analitza/list.h"
#include "analitza/variable.h"
#include "analitza/container.h"
#include "analitza/apply.h"
#include "analitza/expressiontypechecker.h"
#include "analitza/expression.h"
#include "operationscompiler.h"
#include "typecompiler.h"

Q_DECLARE_METATYPE(llvm::Value*);

//TODO better names
static llvm::IRBuilder<> buildr(llvm::getGlobalContext());
static std::map<QString, llvm::Value*> NamedValues;

using namespace Analitza;

ExpressionCompiler::ExpressionCompiler(llvm::Module* module, Variables* vars)
	: m_module(module)
	, m_vars(vars)
{
	//TODO ad variables to m_bvartypes
}

llvm::Value *ExpressionCompiler::compileExpression(Object* expression, const QMap< QString, Analitza::ExpressionType >& bvartypes)
{
	//NOTE we nned to copy the input here since it will be managed externally and a new instance of Expression will hold and insert
	//the @expression into its shareddataptr
	return compileExpression(Analitza::Expression(expression->copy()), bvartypes);
}

llvm::Value *ExpressionCompiler::compileExpression(const Analitza::Expression& expression, const QMap< QString, Analitza::ExpressionType >& bvartypes)
{
	m_bvartypes = TypeCompiler::compileTypes(bvartypes);
	
	if (expression.isLambda()) {
		ExpressionTypeChecker check(m_vars);
		check.initializeVars(bvartypes);
		
		ExpressionType rett = check.check(expression.lambdaBody());
		//TODO check if rett (return type) has error ret.Errors
// 		qDebug() << "FUNT RET TYPE "<< rett.toString();
		m_rettype = TypeCompiler::compileType(rett);
	}
	
	return expression.tree()->accept(this).value<llvm::Value*>();
}


QVariant ExpressionCompiler::visit(const Analitza::Ci* var)
{
	//TODO chack in variables too ... since we are playing by efault with stack vars
// 	NamedValues[var->name()]->dump();
	return QVariant::fromValue<llvm::Value*>(NamedValues[var->name()]);
}

QVariant ExpressionCompiler::visit(const Analitza::Operator* op)
{
	//NOTE see visit apply implementation is there
	return QVariant();
}

QVariant ExpressionCompiler::visit(const Analitza::Vector* vec)
{
	//TODO
	return QVariant();
}

QVariant ExpressionCompiler::visit(const Analitza::Matrix* m)
{
	//TODO
	return QVariant();
}

QVariant ExpressionCompiler::visit(const Analitza::MatrixRow* mr)
{
	//TODO
	return QVariant();
}

QVariant ExpressionCompiler::visit(const Analitza::List* vec)
{
	//TODO
	return QVariant();
}

QVariant ExpressionCompiler::visit(const Analitza::Cn* val)
{
	llvm::Value *ret = 0;
	
	switch(val->format()) {
		//TODO
// 		case Analitza::Cn::Boolean: // ret = llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(sizeof(bool)*8, val->intValue(), false)); break;
// 		case Analitza::Cn::Char: //ret = llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(sizeof(char)*8, val->intValue(), false)); break;
		case Analitza::Cn::Integer: //ret = llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(sizeof(int)*8, val->intValue(), true)); break;
		case Analitza::Cn::Real: ret = llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(val->value())); break;
		case Analitza::Cn::Complex: {
			llvm::Type *rawtypes[2] = {llvm::Type::getDoubleTy(llvm::getGlobalContext()), llvm::Type::getDoubleTy(llvm::getGlobalContext())};
			llvm::ArrayRef<llvm::Type*> types = llvm::ArrayRef<llvm::Type*>(rawtypes, 2);
			llvm::StructType *complextype = llvm::StructType::get(llvm::getGlobalContext(), types, true);
			
			std::complex<double> complexval = val->complexValue();
			llvm::Constant *rawvalues[2] = {llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(complexval.real())),
				llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(complexval.imag()))};
			llvm::ArrayRef<llvm::Constant*> values = llvm::ArrayRef<llvm::Constant*>(rawvalues, 2);
			
			ret = llvm::ConstantStruct::get(complextype,values);
		}	break;
	}
	
	return QVariant::fromValue((llvm::Value*)ret); //TODO better casting using LLVM API
}

QVariant ExpressionCompiler::visit(const Analitza::Apply* c)
{
	llvm::Value *ret = 0;
	
	const Operator::OperatorType op = c->firstOperator().operatorType();
	QString error; //TODO error management
	
	switch(c->countValues()) {
		case 1: {
			Object *val = c->at(0);
// 			qDebug() << "DUMPPPP " << apply->firstOperator().toString();
			llvm::Value *valv =val->accept(this).value<llvm::Value*>();
// 			valv->dump();
// 			valv->getType()->dump();
// 			qDebug() << "ENNNDDDD";
			
			ret = OperationsCompiler::compileUnaryOperation(buildr.GetInsertBlock(), op, Object::value, valv, error);
		}	break;
		case 2: {
			Object *val1 = c->at(0);
// 			qDebug() << "obj1 " << val1->toString();
			llvm::Value *valv1 =val1->accept(this).value<llvm::Value*>();
// 			valv1->getType()->dump();
			Object *val2 = c->at(1);
// 			qDebug() << "obj2 " << val2->toString();
// 			qDebug() << "DUMPPPP " << apply->firstOperator().toString();
			llvm::Value *valv2 =val2->accept(this).value<llvm::Value*>();
// 			valv2->getType()->dump();
// 			valv->dump();
// 			valv->getType()->dump();
// 			qDebug() << "ENNNDDDD";
			
			ret = OperationsCompiler::compileBinaryOperation(buildr.GetInsertBlock(), op, Object::value, Object::value, valv1, valv2, error);
		}	break;
	}
	
	return QVariant::fromValue((llvm::Value*)ret); //TODO better casting using LLVM API
}

QVariant ExpressionCompiler::visit(const Analitza::Container* c)
{
	llvm::Value *ret = 0;
	switch(c->containerType()) {
		case Analitza::Container::piecewise: {
			//NOTE we need to order the execution
			//From MathML documentation: It should be noted that no "order of execution" is implied 
			//by the ordering of the piece child elements within piecewise. It is the responsibility 
			//of the author to ensure that the subsets of the function domain defined by the second 
			//children of the piece elements are disjoint, or that, where they overlap, the values of 
			//the corresponding first children of the piece elements coincide. If this is not the case, 
			//the meaning of the expression is undefined. 
			
			llvm::Function *TheFunction = buildr.GetInsertBlock()->getParent();
			QList<Container*> pieces;
			Container* otherwise = 0; //we have always one and only one otherwise
			
			foreach(Object *o, c->m_params) {
				Container *p=static_cast<Container*>(o); //piecewise part
				Q_ASSERT( o->isContainer() && (p->containerType()==Container::piece || p->containerType()==Container::otherwise) );
				
				if (p->containerType()==Container::piece) {
					pieces.append(p);
				}
				else if (p->containerType()==Container::otherwise) {
					otherwise = p;
				}
			}
			
			llvm::Value *elseif = llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(0.0));
			
			//basic idea to build IR from piecewise: if () else if else if else ... else otherwise
			for (int i = 0; i < pieces.size(); ++i) {
				Container *currpiece = pieces[i];
				Apply *currthen = (Apply*)currpiece->m_params[0];
				Apply *currcond = (Apply*)currpiece->m_params[1];
				
				llvm::Value *CondV = currcond->accept(this).value<llvm::Value*>();
				
				llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "then", TheFunction);
				llvm::BasicBlock *ElseBB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "else");
				llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "ifcont");
				
				buildr.CreateCondBr(CondV, ThenBB, ElseBB);
				buildr.SetInsertPoint(ThenBB);
				
				llvm::Value * ThenV = currthen->accept(this).value<llvm::Value*>();
				//TODO ASSERT IR ThenValue
				buildr.CreateBr(MergeBB);
				
				TheFunction->getBasicBlockList().push_back(ElseBB);
				buildr.SetInsertPoint(ElseBB);
				
				llvm::Value *ElseV = elseif;
				buildr.CreateBr(MergeBB);
				// Codegen of 'Else' can change the current block, update ElseBB for the PHI.
// 				ElseBB = buildr.GetInsertBlock();
				
				TheFunction->getBasicBlockList().push_back(MergeBB);
				buildr.SetInsertPoint(MergeBB);
				
				llvm::PHINode *PN = buildr.CreatePHI(llvm::Type::getDoubleTy(llvm::getGlobalContext()), 2, "iftmp");
				PN->addIncoming(ThenV, ThenBB);
				PN->addIncoming(ElseV, ElseBB);
// 				
				elseif = PN;
			}
			
			ret = elseif;
			
			TheFunction->dump();
			
		}	break;
		case Analitza::Container::piece: {
			//NOTE implementation is in piecewise
		}	break;
		case Analitza::Container::declare:{
// 			Q_ASSERT(c->m_params.first()->type()==Object::variable);
// 			Ci* var = static_cast<Ci*>(c->m_params.first());
// 			
// 			m_calculating.append(var->name());
// 			c->m_params.last()->accept(this);
// 			m_calculating.removeLast();
// 			
// 			current=tellTypeIdentity(var->name(), current);
		}	break;
		case Analitza::Container::lambda: {
			const QStringList bvars = c->bvarStrings();
			
			std::vector<llvm::Type*> tparams = m_bvartypes.values().toVector().toStdVector();
			
// 			tparams.at(0)->dump();
// 			tparams.at(1)->dump();
			
// 			buildr.CreateRet(rawret);
			//BEGIN
			
			//END
			
			//TODO we need the return type
			llvm::FunctionType *FT = llvm::FunctionType::get(m_rettype, tparams, false);
			
			llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "mylam", m_module);//TODO count how many lambdas we have generated i.e mylam0, mylam1 ...
			
			//TODO better code
			int Idx = 0;
			for (llvm::Function::arg_iterator AI = F->arg_begin(); Idx != bvars.size(); ++AI, ++Idx) {
				AI->setName(bvars[Idx].toStdString());
// 				AI->dump();
				NamedValues[bvars[Idx]] = AI;
			}
			
			llvm::BasicBlock *BB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", F);
// 			trik->moveBefore(BB);
// 			BB->dump();
			buildr.SetInsertPoint(BB);
			buildr.CreateRet(c->m_params.last()->accept(this).value<llvm::Value*>());
			
// 			QVariant al = QVariant::fromValue<llvm::Value*>(F);
			
// 			BB->dump();
			llvm::verifyFunction(*F);
			
			ret = F;
		}	break;
		case Analitza::Container::otherwise: {
			//NOTE implementation is in piecewise
		}	break;
		case Analitza::Container::math:
		case Analitza::Container::none:
		case Analitza::Container::downlimit:
		case Analitza::Container::uplimit:
		case Analitza::Container::bvar:
		case Analitza::Container::domainofapplication: {
			Q_ASSERT(c->constBegin()+1==c->constEnd());
			ret = (*c->constBegin())->accept(this).value<llvm::Value*>();
		}	break;
	}
	
	return QVariant::fromValue((llvm::Value*)ret); //TODO better casting using LLVM API
}

QVariant ExpressionCompiler::visit(const Analitza::CustomObject*)
{
	return QVariant();//"CustomObject";
}

QVariant ExpressionCompiler::visit(const Analitza::None* )
{
	return QVariant();
}
