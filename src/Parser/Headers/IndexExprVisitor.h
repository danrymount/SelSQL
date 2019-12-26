//
// Created by sapiest on 26.12.2019.
//

#ifndef SELSQL_INDEXEXPRVISITOR_H
#define SELSQL_INDEXEXPRVISITOR_H
#include "../../Utils/Headers/ActionsUtils.h"
#include "../Nodes/ExpressionsNodes/CompareNodes/EqualsNode.h"
#include "../Nodes/ExpressionsNodes/CompareNodes/LessEqNode.h"
#include "../Nodes/ExpressionsNodes/CompareNodes/LessNode.h"
#include "../Nodes/ExpressionsNodes/CompareNodes/MoreEqNode.h"
#include "../Nodes/ExpressionsNodes/CompareNodes/MoreNode.h"
#include "../Nodes/ExpressionsNodes/CompareNodes/NoEqualsNode.h"
#include "../Nodes/ExpressionsNodes/ExprNode.h"
#include "../Nodes/ExpressionsNodes/IndentExprNode.h"
#include "../Nodes/ExpressionsNodes/LogicNodes/AndLogicNode.h"
#include "../Nodes/ExpressionsNodes/LogicNodes/NotLogicNode.h"
#include "../Nodes/ExpressionsNodes/LogicNodes/OrLogicNode.h"
#include "../Nodes/ValuesNodes/NullValueNode.h"
#include "TreeVisitor.h"
class IndexExprVisitor : public TreeVisitor {
   public:
    void visit(ExprNode* node) override {
        if (node->getChild()) {
            for (auto& val : values) {
                ans.emplace_back(val.second);
            }
            node->getChild()->accept(this);
        }
    }

    std::string executeLeftArith(BaseExprNode* node) {
        if (node->getLeft()->getBaseValue().empty()) {
            node->getLeft()->accept(this);
            auto res = node->getLeft()->getBaseValue();
            node->getLeft()->setValue("");
            return res;
        }
        return node->getLeft()->getBaseValue();
    }

    std::string executeRightArith(BaseExprNode* node) {
        if (node->getRight()->getBaseValue().empty()) {
            node->getRight()->accept(this);
            auto res = node->getRight()->getBaseValue();
            node->getRight()->setValue("");
            return res;
        }
        return node->getRight()->getBaseValue();
    }

    void visit(AndLogicNode* node) override {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
        auto left = allans.back();
        allans.pop_back();
        auto right = allans.back();
        allans.pop_back();
        ans.clear();
        std::sort(left.begin(), left.end());
        std::sort(right.begin(), right.end());
        std::set_intersection(left.begin(), left.end(), right.begin(), right.end(), back_inserter(ans));
        allans.emplace_back(ans);
    }

    void visit(OrLogicNode* node) override {
        node->getLeft()->accept(this);
        node->getRight()->accept(this);
        auto left = allans.back();
        allans.pop_back();
        auto right = allans.back();
        allans.pop_back();
        ans.clear();
        std::sort(left.begin(), left.end());
        std::sort(right.begin(), right.end());
        std::set_union(left.begin(), left.end(), right.begin(), right.end(), back_inserter(ans));
        allans.emplace_back(ans);
    }

    void visit(NotLogicNode* node) override {
        node->getLeft()->accept(this);
        auto left = allans.back();
        allans.pop_back();
        ans.clear();
        for (auto& val : values) {
            bool isExist = false;
            for (auto& a : left) {
                if (a == val.second) {
                    isExist = true;
                    break;
                }
            }
            if (!isExist) {
                ans.emplace_back(val.second);
            }
        }
        allans.emplace_back(ans);
        // node->setResult(not executeLeftLogic(node));
    }

    void visit(NullValueNode* node) override { node->setValue("null"); }

    void visit(MoreNode* node) override {
        auto left = executeLeftArith(node);
        auto right = executeRightArith(node);
        auto res = left.empty() ? right : left;
        auto it = values.upper_bound(res);
        ans.clear();
        for (auto v = it; v != values.end(); v++) {
            ans.emplace_back(v->second);
        }
        allans.emplace_back(ans);
    }

    void visit(EqualsNode* node) override {
        auto left = executeLeftArith(node);
        auto right = executeRightArith(node);
        auto res = left.empty() ? right : left;
        res = std::to_string(std::stoi(res));
        auto it = values.equal_range(res);
        ans.clear();
        for (auto v = it.first; v != it.second; v++) {
            ans.emplace_back(v->second);
        }
        allans.emplace_back(ans);
    }
    void visit(NoEqualsNode* node) override {
        auto left = executeLeftArith(node);
        auto right = executeRightArith(node);
        auto res = left.empty() ? right : left;
        res = std::to_string(std::stoi(res));
        auto it = values.equal_range(res);
        ans.clear();
        for (auto& value : values) {
            bool isEquals = false;
            for (auto inner = it.first; inner != it.second; inner++) {
                if (inner->first == value.first) {
                    isEquals = true;
                    break;
                }
            }
            if (isEquals) {
                continue;
            }
            ans.emplace_back(value.second);
        }
        allans.emplace_back(ans);
    }

    void visit(MoreEqNode* node) override {
        auto left = executeLeftArith(node);
        auto right = executeRightArith(node);
        auto res = left.empty() ? right : left;
        auto it = values.lower_bound(res);
        ans.clear();
        for (auto v = it; v != values.end(); v++) {
            ans.emplace_back(v->second);
        }
        allans.emplace_back(ans);
    }

    void visit(LessEqNode* node) override {
        auto left = executeLeftArith(node);
        auto right = executeRightArith(node);
        auto res = left.empty() ? right : left;
        auto it = values.upper_bound(res);
        ans.clear();
        for (auto v = values.begin(); v != it; v++) {
            ans.emplace_back(v->second);
        }
        allans.emplace_back(ans);
    }

    void visit(LessNode* node) override {
        auto left = executeLeftArith(node);
        auto right = executeRightArith(node);
        auto res = left.empty() ? right : left;
        auto it = values.lower_bound(res);
        ans.clear();
        for (auto v = values.begin(); v != it; v++) {
            if (v == it) {
                break;
            }
            ans.emplace_back(v->second);
        }
        allans.emplace_back(ans);
    }

    void setValues(std::multimap<std::string, int> _vals) { values = _vals; }

    std::vector<int> gerAns() {
        if (allans.empty()) {
            return ans;
        } else {
            return allans[0];
        }
    }

   private:
    bool isNull = false;
    bool result = true;
    std::multimap<std::string, int> values;
    std::vector<int> ans;
    std::vector<std::vector<int>> allans;
};
#endif  // SELSQL_INDEXEXPRVISITOR_H
