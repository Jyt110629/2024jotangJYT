#include "../../include/ir/opt/DCE.hpp"

bool DCE::Run() {
 // 遍历当前模块中的所有函数
    for (auto &func : Singleton<Module>().GetFuncTion()) {
        // 遍历每个函数中的基本块
        for (auto bb : (*func)) {
            // 遍历基本块中的每个指令
            for (auto inst : (*bb)) {
                // 检查指令是否没有副作用
                if (!HasSideEffect(inst)&&inst->GetUserList().IsEmpty() ) {
                        RemoveInst(inst);
                }
            }
        }
    }
  return true;
}

bool DCE::HasSideEffect(User *inst) {
    // 获取指令类型
    auto opcode = inst->GetOpcode();

    // 检查是否为无副作用的指令
    switch (opcode) {
        case Opcode::Add:
        case Opcode::Sub:
        case Opcode::Mul:
        case Opcode::Div:
        case Opcode::And:
        case Opcode::Or:
        case Opcode::Not:
        case Opcode::Eq:
        case Opcode::Ne:
        case Opcode::Gt:
        case Opcode::Lt:
        case Opcode::BitwiseAnd:
        case Opcode::BitwiseOr:
        case Opcode::BitwiseNot:
        case Opcode::Assign: // 仅在没有其他使用时
        case Opcode::TypeCast:
            return false; // 无副作用
        
        // 其他有副作用的指令
        case Opcode::Store: // 存储操作
        case Opcode::Load: // 加载操作可能影响状态
        case Opcode::Call: // 函数调用
        case Opcode::Free: // 内存释放
        case Opcode::Malloc: // 动态分配内存
        case Opcode::Print: // 打印输出
        case Opcode::Throw: // 抛出异常
            return true; // 有副作用
        
        default:
            // 处理未覆盖的指令类型
            return true; // 假设未知指令有副作用
    }
}


void DCE::RemoveInst(User *inst) {
  // 确保指令没有用户
    if (inst->GetUserList().IsEmpty() ) {
        // 遍历指令的usee，更新它们的 UseList
        for (auto use : inst->GetUseList()) {
            auto a=use->GetValue();
            a->RemoveUser(inst);
            
        }
        // 删除指令
        delete inst;
    }
}