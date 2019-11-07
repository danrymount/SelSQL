//
// Created by sapiest on 07.10.2019.
//

#ifndef SELSQL_VARIABLE_H
#define SELSQL_VARIABLE_H

#include <utility>

#include "../../Headers/CommonUtils.h"

class Variable {
   public:
    typedef std::vector<Constraint> ConstraintsVector;

    Variable() = default;

    Variable(Type _type, ConstraintsVector _constraints) : type(_type), constraints(std::move(_constraints)){};
    explicit Variable(Type type_) : type(type_) {}
    Variable(Type _type, int _size, ConstraintsVector _constraints)
                                                                                                        : type(_type),
                                                                                                          size(_size),
                                                                                                          constraints(std::move(_constraints)){};

    int addConstraint(Constraint constraint) {
        for (auto &constraint_ : constraints) {
            if (constraint_ == constraint)
                return 1;
        }
        constraints.push_back(constraint);
        return 0;
    }

    ConstraintsVector getConstraints() { return constraints; }

    void setConstraints(ConstraintsVector &constraints_) { this->constraints = constraints_; }

    Type type;
    int size = 0;

   private:
    std::vector<Constraint> constraints;
};

#endif  // SELSQL_VARIABLE_H
