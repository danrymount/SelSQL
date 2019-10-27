//
// Created by sapiest on 07.10.2019.
//

#ifndef SELSQL_VARIABLE_H
#define SELSQL_VARIABLE_H

#include "../../Headers/CommonUtils.h"

class Variable {
   public:
    typedef std::vector<Constraint> ConstraintsVector;

    Variable() = default;

    Variable(Type _type, ConstraintsVector _constraints) : type(_type), constraints(_constraints){};
    explicit Variable(Type type_) : type(type_) {}
    Variable(Type _type, int _size): type(_type), size(_size){};

    int addConstraint(Constraint constraint) {
        for (auto &constraint_ : constraints) {
            if (constraint_ == constraint)
                return 1;
        }
        constraints.push_back(constraint);
        return 0;
    }

    ConstraintsVector getConstraints() { return constraints; }

    ConstraintsVector getConstraints() const { return constraints; }

    void setConstraints(ConstraintsVector &constraints_) { this->constraints = constraints_; }

    Type type;

   private:
    std::vector<Constraint> constraints;
    int size = 0;
};

#endif  // SELSQL_VARIABLE_H
