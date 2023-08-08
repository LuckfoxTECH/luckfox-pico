/*
 * Copyright (c) 2019-2022 Rockchip Eletronics Co., Ltd.
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
 */

#ifndef RKCAM_SHARED_ITEM_POOL_H
#define RKCAM_SHARED_ITEM_POOL_H

#include <iostream>
#include <typeinfo>

#include "safe_list.h"
#include "buffer_pool.h"

using namespace XCam;

namespace RkCam {

class SharedItemBase : public BufferProxy {
public:
    explicit SharedItemBase (const SmartPtr<BufferData> &data):BufferProxy(data) {}
    virtual ~SharedItemBase () = default;

    void setType(uint32_t type) { _type = type; }
    void setId(uint32_t id) { _id = id; }

    int getType() { return _type; }
    uint32_t getId() { return _id; }

protected:
    XCAM_DEAD_COPY (SharedItemBase);

    uint32_t _type = -1;
    uint32_t _id = -1;
};

class RkAiqFullParams;
class RkAiqIspStats;
typedef struct RkAiqSofInfoWrapper_s RkAiqSofInfoWrapper_t;

template<typename T>
class SharedItemProxy : public SharedItemBase
{
public:
    explicit SharedItemProxy(const SmartPtr<T> &data) : SharedItemBase(data), _data(data) {};
    virtual ~SharedItemProxy() {
        check();
        _data.release();
        LOG1_ANALYZER("Release item : %s", typeid(T).name());
    };

    template <class U = T>
    typename std::enable_if<(std::is_same<U, RkAiqFullParams>::value ||
                             std::is_same<U, RkAiqIspStats>::value ||
                             std::is_same<U, RkAiqSofInfoWrapper_t>::value),
                            bool>::type
    check() {
        _data->reset();
        return true;
    }

    template <class U = T>
    typename std::enable_if<!(std::is_same<U, RkAiqFullParams>::value ||
                              std::is_same<U, RkAiqIspStats>::value ||
                              std::is_same<U, RkAiqSofInfoWrapper_t>::value),
                            bool>::type
    check() {
        return false;
    }

    SmartPtr<T> &data() {
        return _data;
    }

    uint8_t *map () {
        return (uint8_t *)_data.ptr();
    }
private:
    SmartPtr<T>       _data;
    XCAM_DEAD_COPY (SharedItemProxy);
};

template<typename T>
class SharedItemPool
    : public BufferPool
{
    friend class SharedItemProxy<T>;

public:
    explicit SharedItemPool(const char* name, uint32_t max_count = 8);
    virtual ~SharedItemPool();

    SmartPtr<SharedItemProxy<T>> get_item();
    int8_t init(uint32_t max_count = 8);
    bool has_free_items () {
        return has_free_buffers();
    }
private:
    XCAM_DEAD_COPY (SharedItemPool);

protected:
    const char*              _name;
    uint32_t                 _max_count;
    virtual SmartPtr<BufferData> allocate_data (const VideoBufferInfo &buffer_info);
    virtual SmartPtr<BufferProxy> create_buffer_from_data (SmartPtr<BufferData> &data);
};

};

#include "shared_item_pool.cpp"

#endif //RKCAM_SHARED_ITEM_POOL_H


