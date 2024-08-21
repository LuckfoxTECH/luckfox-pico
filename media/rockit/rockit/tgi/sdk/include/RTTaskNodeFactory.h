/*
 * Copyright 2020 Rockchip Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *  author: <martin.cheng@rock-chips.com>
 *    date: 2020-04-23
 *  module: task node factory
 *
 */

#ifndef SRC_RT_TASK_TASK_NODE_RTTASKNODEFACTORY_H_
#define SRC_RT_TASK_TASK_NODE_RTTASKNODEFACTORY_H_

#include <list>

#include "rt_header.h"
#include "RTTaskNodeDef.h"

class RTTaskNode;
class RTTaskNodeToken;
class RTTaskNodeFactory;
typedef RTTaskNode* (*RTNodeCreate)();
typedef struct _RTNodeStub {
    const INT32  mUid;
    const char  *mName;
    const char  *mVersion;
    RTNodeCreate mCreateObj;
    RTPadCaps    mCapsSrc;
    RTPadCaps    mCapsSink;
} RTNodeStub;

#define REGISTRY_STATIC_VAR_INNER(var_name, line) var_name##_##line##__
#define REGISTRY_STATIC_VAR(var_name, line) \
    REGISTRY_STATIC_VAR_INNER(var_name, line)

#define RT_NODE_FACTORY_CREATE_NODE(idName) \
    RTTaskNodeFactory::instance()->createNode(idName)

#define RT_NODE_FACTORY_GET_UID_BY_NAME(name) \
    RTTaskNodeFactory::instance()->getUidByName(name)

#define RT_NODE_FACTORY_GET_STUB_BY_UID(uid) \
    RTTaskNodeFactory::instance()->getStubByUid(uid)

#define RT_NODE_FACTORY_REGISTER_STUB(stub) \
    auto REGISTRY_STATIC_VAR(stub, __LINE__) = RTTaskNodeToken(stub);

class RTTaskNodeFactory {
 public:
    RTTaskNodeFactory();
    virtual ~RTTaskNodeFactory();
    static RTTaskNodeFactory* instance();

 public:
    RT_RET      registerStub(RTNodeStub* stub);
    RTTaskNode* createNode(const char* name);
    RTTaskNode* createNode(RTStubUid stubKey);
    RTTaskNode* createNode(RTPadCaps* caps);
    RTTaskNode* createNode(RTNodeStub* stub);
    RTStubUid   getUidByName(const char* name);
    RTNodeStub* getStubByUid(RTStubUid stubKey);

 private:
    std::list<RTNodeStub*>    mStubs;
};

class RTTaskNodeToken {
 public:
    explicit RTTaskNodeToken(const RTNodeStub &stub);
    virtual ~RTTaskNodeToken() {}
 private:
    RTNodeStub *mNodeStub;
};

#endif  // SRC_RT_TASK_TASK_NODE_RTTASKNODEFACTORY_H_
