#include "../../include/ir/opt/ConstantProp.hpp"
#include <cmath>
#include <set>

bool ConstantProp::Run() {
  for(auto bb:(*_func))
  {
    for(auto inst:(*bb))
    {
        bool allConsts = true;
          for(auto &use:inst->Getuselist())//判断usee是否都为常数
          {		
              if (!use->GetValue()->isConst()) {
                        allConsts = false;
                        break;
                    }
                }
      
      if(allConsts)
      {
        auto val = ConstFoldInst(inst);
            if(val)
            {
            inst->RAUW(val);
            inst->EraseFromParent();
            }
      	  }
      	}
      }
  return true;
}

Value *ConstantProp::ConstFoldInst(User *inst) {
  // 确保指令类型可以折叠
  if (auto *binOp = dyn_cast<BinaryOperator>(inst)) {
    Value *lhs = binOp->getOperand(0);
    Value *rhs = binOp->getOperand(1);

    // 检查操作数是否都是常量
    if (auto *lhsConst = dyn_cast<ConstantInt>(lhs)) {
      if (auto *rhsConst = dyn_cast<ConstantInt>(rhs)) {
        // 根据不同的操作符进行处理
        switch (binOp->getOpcode()) {
          case Instruction::Add:
            return ConstantInt::get(lhsConst->getType(), 
                                     lhsConst->getValue() + rhsConst->getValue());
          case Instruction::Sub:
            return ConstantInt::get(lhsConst->getType(), 
                                     lhsConst->getValue() - rhsConst->getValue());
          case Instruction::Mul:
            return ConstantInt::get(lhsConst->getType(), 
                                     lhsConst->getValue() * rhsConst->getValue());
          case Instruction::SDiv:
            // 除以零检查
            if (rhsConst->isZero()) {
              return nullptr; // 或者处理成无效值
            }
            return ConstantInt::get(lhsConst->getType(), 
                                     lhsConst->getValue() / rhsConst->getValue());
          case Instruction::And:
            return ConstantInt::get(lhsConst->getType(), 
                                     lhsConst->getValue() & rhsConst->getValue());
          case Instruction::Or:
            return ConstantInt::get(lhsConst->getType(), 
                                     lhsConst->getValue() | rhsConst->getValue());
          case Instruction::Shl: 
            return ConstantInt::get(lhsConst->getType(), 
                                     lhsConst->getValue() << rhsConst->getValue().getZExtValue());
          case Instruction::Shr: 
            return ConstantInt::get(lhsConst->getType(), 
                                     lhsConst->getValue() >> rhsConst->getValue().getZExtValue());
          case Instruction::Xor:
  			return ConstantInt::get(lhsConst->getType(), 
                           lhsConst->getValue() ^ rhsConst->getValue());
            case Instruction::Not:
              return ConstantInt::get(lhsConst->getType(), 
                                       ~lhsConst->getValue());
            case Instruction::ICmp: // 比较操作
              // 处理特定的比较类型
              if (auto *cmpInst = dyn_cast<ICmpInst>(binOp)) {
                // 进行常量比较
                return ConstantInt::get(Type::getInt1Ty(inst->getContext()), 
                                         lhsConst->getValue() == rhsConst->getValue());
              }
            case Instruction::Select: // 选择指令
              if (auto *condConst = dyn_cast<ConstantInt>(binOp->getCondition())) {
                return condConst->isOne() ? binOp->getTrueValue() : binOp->getFalseValue();
              }

          default:
            break; // 不支持的操作符
        }
      }
    }
  }

  // 对于无法折叠的情况返回 nullptr
  return nullptr;
}
