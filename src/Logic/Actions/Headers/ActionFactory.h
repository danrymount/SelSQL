//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_ACTIONFACTORY_H
#define SELSQL_ACTIONFACTORY_H

#include <map>
#include "BaseAction.h"
#include "CreateAction.h"
#include "DeleteAction.h"
#include "DropAction.h"
#include "InsertAction.h"
#include "SelectAction.h"
#include "ShowCreateAction.h"
#include "UpdateAction.h"
class BaseCreator {
   public:
    virtual BaseAction* create() const = 0;
};

template <class T>
class ActionCreator : public BaseCreator {
   public:
    virtual BaseAction* create() const { return new T(); }
};

class ActionFactory {
    typedef std::map<Action, BaseCreator*> FactoryMap;
    FactoryMap _factory;

   public:
    ActionFactory() {
        this->add<CreateAction>(CREATE);
        this->add<DropAction>(DROP);
        this->add<ShowCreateAction>(SHOW_CREATE);
        this->add<DeleteAction>(DELETE_ACT);
        this->add<InsertAction>(INSERT);
        this->add<SelectAction>(SELECT);
        this->add<UpdateAction>(UPDATE);
    }

    template <class T>
    void add(const Action& id) {
        auto it = _factory.find(id);
        if (it == _factory.end())
            _factory[id] = new ActionCreator<T>();
    }

    BaseAction* create(const Action& id) {
        auto it = _factory.find(id);
        if (it != _factory.end())
            return it->second->create();
    }
};

#endif  // SELSQL_ACTIONFACTORY_H
